#include "gl_framework.h"
#include "shader_util.hpp"
#include "commonData.h"
#include "keyboard.h"
#include "main.h"
#include <time.h>
#include <pthread.h>
#include <omp.h>
#include <sys/time.h>


//#include "viennacl/vector.hpp"

#define WG  //WG - With Graphics
//#undef WG

int grid_size;
int nthreads;
#ifndef WG
int main(int argc, char** argv)
{
	grid_size = GRID_SIZE;
	nthreads = NTHREADS;	
	if(argc>=2){
	nthreads = atoi(argv[1]);
	grid_size = atoi(argv[2]);
	}
 	omp_set_num_threads(nthreads);
	struct timeval tt1, tt2;
	initMain();
	int i=pthread_getconcurrency();
	int it=0;
	int ni= 0;
	while(ni++<NO_OF_ITERATIONS){
		gettimeofday(&tt1, NULL);
		animate();
		gettimeofday(&tt2, NULL);
		int milliSeconds = (tt2.tv_sec - tt1.tv_sec) * 1000 + (tt2.tv_usec - tt1.tv_usec)/1000;
		cout<<"Iteration "<<it<<" : "<<milliSeconds<<"ms"<<endl<<endl;
		it++;
	}
	return 0;
}
#endif


void animate()
{
	fluidSim->simulate(timestep);
} 

void initMain(void) 
{
   sGrid->initGridStag();
   fluidSim->init(sGrid);
   fluidSim->initFluidBody(fBT);// 2: indicates dam break center
   print->init(sGrid);
   render->init(sGrid);
   
}

void initParticles()
{
	sGrid->fluidParticles.clear();
}

/*------------------Code Below this is used when rendered is ON---------*/
void renderGL( void ) ;
void preDisplay ( );
void postDisplay ( );
void openGlutWindow ( char* windowName) ;
void reshape ( int w, int h ) ;
void idleFun();

#ifdef WG


GLuint shaderProgram;
GLuint vbo, vao;

void initShadersGL(void)
{
  std::string vertex_shader_file("simple_vs.glsl");
  std::string fragment_shader_file("simple_fs.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  
}

void initVertexBufferGL(void)
{
  float points[]={};
    
  //Ask GL for a Vertex Buffer Object (vbo)
   glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer - 9 float values, start pointer and sttic data
  //glBufferData (GL_ARRAY_BUFFER, 3 * sizeof (float), points, GL_STATIC_DRAW);
  //Ask GL for a Vertex Attribute Object (vao)
   glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
   glBindVertexArray (vao);
  //Enable the vertex attribute
   glEnableVertexAttribArray (0);
  //This the layout of our first vertex buffer
  //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats 
   glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glBindVertexArray(0);
}



///////////////////////////GLFW CODE/////////////////////////////////////
/**/
int main (int argc, char *argv[]) 
{
  grid_size = GRID_SIZE;
  nthreads = NTHREADS;	
  if(argc>=2){
    nthreads = atoi(argv[1]);
    grid_size = atoi(argv[2]);
  }
  omp_set_num_threads(nthreads);
   
 //! The pointer to the GLFW window
  GLFWwindow* window;
 
  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;
  
  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  //This is for MacOSX - can be omitted otherwise
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 


  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(winSizeX, winSizeY, " Liquid_Simulator-LevelSet+Surface", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }

  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initShadersGL();
  // initVertexBufferGL();
 
  initMain();
  int i=pthread_getconcurrency();
  
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
      glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT);
      glfwSwapBuffers(window);
      //glClear(GL_COLOR_BUFFER_BIT);
      //      glfwSwapBuffers(window);

      // Render here
      renderGL();

      // Swap front and back buffers
      //glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

/**/
/////////////////////////////////////////////////////////////////////////

#endif


void renderGL(void){
  GLFWwindow *window=glfwGetCurrentContext();
 	preDisplay();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray (vao);
 	static bool flag[10]={false,false,false,false,false,true};//boundary grid particles surface vector mat

	char output1 = ' ';
	bool anyUpdation = false;
        

	switch(output1){
		case'~':
			flag[0] = !flag[0];
			//render->renderBoundary
			break;
		case '!':
			flag[1] = !flag[1];
			//render->renderGrid();
			break;
		case '@':
			flag[2] = !flag[2];
			//render->renderParticles();
			break;
		case '#':
			flag[3] = !flag[3];
			//render->renderSurfaceBoundary();
			break;
		case '$':
			flag[4] = !flag[4];
			//render->renderVector2D(sGrid->u,sGrid->v);
			break;
		case '%':
			flag[5] = !flag[5];
			//render->renderMat(sGrid->distanceLevelSet,1);
			break;
		/*case '^':
			flag[6] = !flag[6];
			//render->renderMat(sGrid->isFluidBoundary,1);
			break;*/
	}	
	if(anyUpdation){
		cout<<"Flags :"<<" ~"<<flag[0]<<" !"<<flag[1]<<" @"<<flag[2]<<" #"<<flag[3]<<
			         " $"<<flag[4]<<" %"<<flag[5]/*<<" ^"<<flag[6]*/<<"   +"<<endl;
		anyUpdation = false;
		}
	extern int swich;
	if(swich==0){
	  // render->renderBoundary();
	  render->renderGrid();
	  render->renderParticles();
	  //render->renderSurfaceBoundary();
	}
	else if(swich==1){
	  	render->renderMat(sGrid->p,2);
	}
	else if(swich==2){
		render->renderMat(sGrid->distanceLevelSet,2);
	}
	else if(swich==3){
		render->renderBoundary();
		render->renderGrid();
		render->renderVector2D(sGrid->u,sGrid->v); 
		}

	
	/*if(flag[0])
		render->renderBoundary();
	if(flag[1])
		render->renderGrid();
	if(flag[2])
		render->renderParticles();
	if(flag[3])
		render->renderSurfaceBoundary();
	if(flag[4])
		render->renderVector2D(sGrid->u,sGrid->v); */
	//if(flag[5])
		//render->renderMat(sGrid->distanceLevelSet,2);
		//render->renderMat(sGrid->p,2);
	/*if(flag[6])
		render->renderMat(sGrid->isFluidBoundary,1);
*/
	output1 = ' ';
	idleFun();
	// 	postDisplay();
	  glfwSwapBuffers(window);
}

void preDisplay()
{
	glViewport ( 0, 0, winSizeX, winSizeY);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho( 0, zoomFactor, 0, zoomFactor	, 0,1 ); //better to use ortho..
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT);
}

void idleFun ( void )
{
	extern bool isPause ;
	if(!isPause){
	struct timeval tt1, tt2;
	static int it = 0;
	static struct timeval tt3, tt4;
	gettimeofday(&tt4, NULL);

	gettimeofday(&tt1, NULL);
	animate();
	gettimeofday(&tt2, NULL);
	int milliSeconds2 = (tt4.tv_sec - tt3.tv_sec) * 1000 + (tt4.tv_usec - tt3.tv_usec)/1000;
	cout<<"Render Time Frame   "<<it-1<<" : "<<milliSeconds2<<"ms"<<endl;

	int milliSeconds = (tt2.tv_sec - tt1.tv_sec) * 1000 + (tt2.tv_usec - tt1.tv_usec)/1000;
	cout<<"Iteration "<<it<<" : "<<milliSeconds<<"ms"<<endl<<endl;
	gettimeofday(&tt3, NULL);
	
	it++;

	////////////FIGURE OUT THESE FUNCTIONS///////////////////////
	//glutSetWindow ( winId );
	//glutPostRedisplay ( );
	}
}
/**/
void postDisplay()
{
  GLFWwindow* window=glfwGetCurrentContext();
   glfwSwapBuffers(window);
}


/******** UPTO HERE********************/
/*
void idleFun ( void )
{
	extern bool isPause ;
	if(!isPause){
	struct timeval tt1, tt2;
	static int it = 0;
	static struct timeval tt3, tt4;
	gettimeofday(&tt4, NULL);

	gettimeofday(&tt1, NULL);
	animate();
	gettimeofday(&tt2, NULL);
	int milliSeconds2 = (tt4.tv_sec - tt3.tv_sec) * 1000 + (tt4.tv_usec - tt3.tv_usec)/1000;
	cout<<"Rander Time Frame   "<<it-1<<" : "<<milliSeconds2<<"ms"<<endl;

	int milliSeconds = (tt2.tv_sec - tt1.tv_sec) * 1000 + (tt2.tv_usec - tt1.tv_usec)/1000;
	cout<<"Iteration "<<it<<" : "<<milliSeconds<<"ms"<<endl<<endl;
	gettimeofday(&tt3, NULL);
	
	it++;
	glutSetWindow ( winId );
	glutPostRedisplay ( );
	}
}
*
void reshape (int w, int h)
{
   glutSetWindow(winId);
   glutReshapeWindow(w ,h);
   winSizeX = w;
   winSizeY = h;
}
/*
void preDisplay()
{
 glViewport ( 0, 0, winSizeX, winSizeY);
 glMatrixMode (GL_PROJECTION);
 glLoadIdentity ();
 glOrtho( 0, zoomFactor, 0, zoomFactor	, 0,1 ); //better to use ortho..
 glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
 glClear(GL_COLOR_BUFFER_BIT);
}

void postDisplay()
{
   glutSwapBuffers();
}
*
void openGlutWindow(char* windowName)
{
   glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );
   glutInitWindowSize (winSizeX, winSizeY);
   glutInitWindowPosition (570, 100);

   winId = glutCreateWindow (&windowName[2]);

   glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);
   glutSwapBuffers();
   glClear(GL_COLOR_BUFFER_BIT);
   glutSwapBuffers();
   glutDisplayFunc(display);
   //   glutSpecialFunc(&SpecialKeyPressed);
   glutKeyboardFunc(&KeyPressed);
   glutReshapeFunc(reshape);
   glutIdleFunc(idleFun);
}
*/


