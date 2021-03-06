#include "gl_framework.h"
#include "Renderer.h"
#include "Printer.h"



//using namespace std;
extern double zoomFactor;
#define DL
//#undef DL

void Renderer :: init(GridStag* grid)
{
  this->sGrid = grid;
  int nX = (this->sGrid)->nX;
  int nY = (this->sGrid)->nY;
  
/*--checkIt--*/
  gridRTX = zoomFactor; //zoomFactorX- main.cpp - glOrtho
  gridRTY = zoomFactor; //zoomFactorY
  gridLBX = 0;
  gridLBY = 0;

  stepX = (gridRTX - gridLBX)/nX;
  stepY = (gridRTX - gridLBX)/nY;
}
void Renderer :: updateGrid(GridStag* grid)
{
  this->sGrid = grid;
}

void Renderer :: initRenderer()
{
  int nX = (this->sGrid)->nX;
  int nY = (this->sGrid)->nY;
  
  gridLBX = -9;
  gridLBY = -9;
  stepX = -1*(gridLBX)*2/nX;
  stepY = -1*(gridLBY)*2/nY;
}


void Renderer :: renderGrid()
{
  int nX = (this->sGrid)->nX;
  int nY = (this->sGrid)->nY;
  
  //cout<<"\nInRenderer:displayGrid"<<nX<< " " <<nY<<" "<<gridLBX<<" "<<gridLBY;
  double gridLBX = this->gridLBX+stepX;
  double gridLBY = this->gridLBY+stepY;
  int fillFlag = 0;
#ifndef  DL
  //BY DEFAULT DL IS NOT ENABLED ________FIND OUT MORE
  for (int i=1;i< nX-1;i++){
    for (int j=1;j< nY-1;j++){
      glColor3f(1,0,0);//set fill color..used if fillFlag=true
      drawSquare(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,fillFlag);//1: Filled grid,0:wire Frame
      gridLBX+=stepX;
    }
    gridLBY += stepY;
    gridLBX = this->gridLBX+stepX;
  }
  std::cout<<"2\n";
#endif
  
#ifdef DL
  //nX = nY=64;
  GLfloat *vertices = new GLfloat[(nX)*2*2*2];
  GLfloat *colors = new GLfloat[(nX)*3*2*2];
  unsigned index=0,c_index=0;
  
  gridLBX = gridLBY = 0;
  for (int i=0;i< nX;i++){
    vertices[index++] = gridLBX;// + stepX*(j-1) ;
    vertices[index++] = gridLBY;// + stepY*(i-1) ;
    colors[c_index++] = 0.2;
    colors[c_index++] = 0.2;
    colors[c_index++] = 0.2;

    vertices[index++] = gridLBX;// + stepX*(j-1) ;
    vertices[index++] = gridLBY+stepY*nX;// + stepY*(i-1) ;
    colors[c_index++] = 0.2;
    colors[c_index++] = 0.2;
    colors[c_index++] = 0.2;

    gridLBX += stepX;
  }
  gridLBX = gridLBY = 0;
  
  for (int i=0;i< nY;i++){
    vertices[index++] = gridLBX;// + stepX*(j-1) ;
    vertices[index++] = gridLBY;// + stepY*(i-1) ;
    colors[c_index++] = 0.2;colors[c_index++] = 0.2;colors[c_index++] = 0.2;

    vertices[index++] = gridLBX+stepY*nX;// + stepX*(j-1) ;
    vertices[index++] = gridLBY;// + stepY*(i-1) ;
    colors[c_index++] = 0.2;colors[c_index++] = 0.20;colors[c_index++] = 0.20;

    gridLBY += stepY;
  }
  
  
  glColor3f(.2,.2,.2); //unComment to show grid lines

#endif
  GLuint vao,vbo,colorbuffer;		
	
  ////////////////////////////////////////////////////////////////
  
  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);

 //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer - 9 float values, start pointer and static data
  glBufferData (GL_ARRAY_BUFFER, index*sizeof (GLfloat), vertices, GL_STATIC_DRAW);

  glGenBuffers(1,&colorbuffer);
  glBindBuffer (GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*c_index, colors, GL_STATIC_DRAW);
 
  glEnableVertexAttribArray (0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  
  glEnableVertexAttribArray(1);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glDrawArrays(GL_LINES, 0, index/2);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindVertexArray(0);
  delete vertices; 
  delete colors;
  //renderBoundary();
}

void Renderer :: renderBoundary()
{
  int nX = (this->sGrid)->nX;
  int nY = (this->sGrid)->nY;
  
  //cout<<"\nInRenderer:displayGrid"<<nX<< " " <<nY<<" "<<gridLBX<<" "<<gridLBY;
  double gridLBX = this->gridLBX;
  double gridLBY = this->gridLBY;
  int fillFlag = 0;
#ifndef  DL
  for (int i=0;i< nX;i++){
    for (int j=0;j< nY;j++){
      if(i==0||j==0||i==nX-1||j==nY-1)
	{
	  glColor3f(0.4,0.4,0.4);
	  drawSquare(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,fillFlag=3);
	  //0,1,2has defined colors..for air liquid and solid cells
	}
      gridLBX+=stepX;
    }
    gridLBY += stepY;
    gridLBX = this->gridLBX;
  }
#endif
#ifdef DL
  
#endif



}

void Renderer :: drawSquare(double lbx,double lby,double rtx,double rty,int fillFlag)
{
  //cout<<"\nRenderer: drawSquare";
  //cout<<"\n.."<<fillFlag;
  switch(fillFlag)
    {
    case AIR:
      glColor3f(0.0,0,0.0);
      break;
    case LIQUID:
      glColor3f(0.1,0.1,0.7); //liquid
      break;
    case SOLID:
      glColor3f(0.2,0.2,0);
      break;
      
    }
  /*following 2 conditions should work..but not working..*/
  //if(fillFlag==LIQUID || fillFlag==SOLID )//uncomment to not render air cells
  //if(fillFlag == LIQUID  )//uncomment to render only liquid..
  
  {
    glBegin(GL_QUADS);
    glVertex2f(lbx, lby);
    glVertex2f(lbx, rty);
    glVertex2f(rtx, rty);
    glVertex2f(rtx, lby);
    glEnd();
  }
  if(!fillFlag)
    {
      glColor3f(.2,.2,.2); //unComment to show grid lines
      glBegin(GL_LINE_LOOP);
      glVertex2f(lbx, lby);
      glVertex2f(lbx, rty);
      glVertex2f(rtx, rty);
      glVertex2f(rtx, lby);
      glEnd();
    }
}
void Renderer :: drawSquareFilled(int li, int lj, int ri, int rj, double fillVal)
{
  double lbx = gridLBX + stepX*li;
  double lby = gridLBY + stepY*lj;
  
  double rtx = gridLBX + stepX*ri;
  double rty = gridLBY + stepY*rj;
  
  /*
  glColor3f(fillVal,0,0);
  glBegin(GL_QUADS);
  glVertex2f(lbx, lby);
  glVertex2f(lbx, rty);
  glVertex2f(rtx, rty);
  glVertex2f(rtx, lby);
  glEnd();
  */
  double vertices[8]= {lbx, rty,
		       lbx, lby,
		       rtx, rty,
		       rtx, lby};
     {
	GLuint vbo,vao;
	glGenVertexArrays (1, &vao);
	//Set it as the current array to be used by binding it
	glBindVertexArray (vao);
	glGenBuffers(1, &vbo);
	
	// Allocate space and upload the data from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindVertexArray(vao);
	//Enable the vertex attribute
	glEnableVertexAttribArray (0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexAttribPointer (0, 2, GL_DOUBLE, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLE_STRIP,0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindVertexArray(0);
	
      }
  

}
void Renderer :: drawSquareFilled(double lbx,double lby,double rtx,double rty,double fillVal)
{
  
  {
    //std::cout<<fillVal<<std::endl;
    //glColor3f(1,1,1);
    ;
    GLfloat colors[12];// = {      0.5f,  0.5f,  0.5f,      };
    
    if(fillVal==-1.0)
      {
	// glColor3f(0,0,0-fillVal);
	colors[0]=0.0;
	colors[1]=0.0;
	colors[2]=0-fillVal;
	colors[3]=0.0;
	colors[4]=0.0;
	colors[5]=0-fillVal;
	colors[6]=0.0;
	colors[7]=0.0;
	colors[8]=0-fillVal;
	colors[9]=0.0;
	colors[10]=0.0;
	colors[11]=0-fillVal;
      }
    if(fillVal>=0)
      {
	//glColor3f(fillVal/6,fillVal/6,fillVal/6);
	colors[0]=fillVal/6;
	colors[1]=fillVal/6;
	colors[2]=fillVal/6;
	colors[3]=fillVal/6;
	colors[4]=fillVal/6;
	colors[5]=fillVal/6;
	colors[6]=fillVal/6;
	colors[7]=fillVal/6;
	colors[8]=fillVal/6;
	colors[9]=fillVal/6;
	colors[10]=fillVal/6;
	colors[11]=fillVal/6;
	
      }
    double vertices[8]= {lbx, rty,
			 lbx, lby,
			 rtx, rty,
			 rtx, lby};
    /*
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(lbx, lby);
    glVertex2f(lbx, rty);
    glVertex2f(rtx, rty);
    glVertex2f(rtx, lby);
    glEnd();
    */
    ///////////////////
    // Create a Vector Buffer Object that will store the vertices on video memory

      {
	GLuint vbo,vao;
	glGenVertexArrays (1, &vao);
	//Set it as the current array to be used by binding it
	glBindVertexArray (vao);
	glGenBuffers(1, &vbo);
	//Vertices
	// Allocate space and upload the data from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
	//COLORS STUFF
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindVertexArray(vao);
	//Enable the vertex attribute
	glEnableVertexAttribArray (0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 2, GL_DOUBLE, GL_FALSE, 0, NULL);

	
	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
			      1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			      3,                                // size
			      GL_FLOAT,                         // type
			      GL_FALSE,                         // normalized?
			      0,                                // stride
			      NULL                          // array buffer offset
			      );
	

	
	
	glDrawArrays(GL_TRIANGLE_STRIP,0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindVertexArray(0);
	
      }

    ///////////////////
    
   
  }
  
  if(0)
    {
      glColor3f(.2,.2,.2); //unComment to show grid lines
      glBegin(GL_LINE_LOOP);
      glVertex2f(lbx, lby);
      glVertex2f(lbx, rty);
      glVertex2f(rtx, rty);
      glVertex2f(rtx, lby);
      glEnd();
      }
}

void Renderer :: renderMat(matrix<double> mat,int mode)
{
  double gridLBX = this->gridLBX;
  double gridLBY = this->gridLBY;
  
  //	int fillFlag = 0;
  
  switch(mode)
    {
    case 1:
      
      for (unsigned int i=0;i<mat.size1();i++){
	for (unsigned int j=0;j<mat.size2();j++){
	  glColor3f(1,0,0);//set fill color..used if fillFlag=true
				/*if(mat(i,j)!=0)
				  fillFlag = 1;
				  else
				  fillFlag = 0;
				*/
	  
				//drawSquare(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,fillFlag);//1: Filled grid,0:wire Frame
				//drawSquare(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,(int)mat(i,j));//1: Filled grid,0:wire Frame
	  gridLBX+=stepX;
	}
	gridLBY += stepY;
	gridLBX = this->gridLBX;
		}
      break;
    case 2 :
      for (unsigned int i=0;i<mat.size1();i++){
	for (unsigned int j=0;j<mat.size2();j++){
	  double fillVal = 0.0;
	  if(i==0||j==0||i==mat.size1()-1||j==mat.size2()-1)
	    {
	      //glColor3f(0.2,0.2,0.2);//set fill color..used if fillFlag=true
	      //drawSquareFilled(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,fillVal);//1: following airs color
	    }
	  else{
	    fillVal  = (mat(i,j));// /max *2.0;
	    drawSquareFilled(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,(double)fillVal);//1: Filled grid,0:wire Frame
	    //drawSquare(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,(int)mat(i,j));//1: Filled grid,0:wire Frame
	    
	  }
	  gridLBX+=stepX;
	}
	gridLBY += stepY;
	gridLBX = this->gridLBX;
      }
      break;
      
      
      
    }
  //renderBoundary();
}


void Renderer :: renderVector2D(matrix<double> u, matrix<double> v)
{
  //float pi = 22/7;
#define pi 22/7
  
  double gridLBX = this->gridLBX;
  double gridLBY = this->gridLBY;
  double stepX = this->stepX;
  double stepY = this->stepY;
  int nX = (this->sGrid)->nX;
  int nY = (this->sGrid)->nY;
  
  matrix<double> vel_ang(nY,nX);
  matrix<double> vel_mag(nY,nX);
  
  for(int i=0;i< nX;i++)
    for(int j=0;j< nY;j++)
      {
	
	
	//if (V(i,j) != 0.0f && U(i,j) != 0.0f) {
	vel_ang(i,j) = (float)atan((float)U(i,j)/(V(i,j)))*180/(pi);
	vel_mag(i,j) = sqrt(U(i,j)*U(i,j)+V(i,j)*V(i,j));
	//		}
      }
  
  
  //	int flag_Quad=0;
  
  static double maxv=0 ;
  
  maxv = MAX(maxv,getMax(vel_mag));
  //maxv = getMax(vel_mag) ;

  
  for (int i=0;i<nX;i++)
    {
      for (int j=0;j<nY;j++)
	{
	  if(i==0||j==0||i==nX-1||j==nY-1)
	    {
	      glColor3f(0.3,0.3,0.3);
	      //drawSquare(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,flag_Quad=4);//1: following airs color
	    }
	  else
	    {
	      glColor3f(1,0,0);
	      //				flag_Quad=0;
	      //drawSquare(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,flag_Quad);//1: on the quad flag
	      drawVectorAngnMag(gridLBX,gridLBY,gridLBX+stepX,gridLBY+stepY,SIGN(U(i,j)),SIGN(V(i,j)),vel_mag(i,j),maxv,vel_ang(i,j));
	    }
	  
	  gridLBX+=stepX;
	}
      gridLBY+=stepY;
      gridLBX = this->gridLBX;
    }
  //renderBoundary();
}



void Renderer :: drawVectorAngnMag(double lbx,double lby,double rtx,double rty,bool usign,bool vsign,double mag,double max,double ang)
{//sign=0  +ve, =1  -ve
/*
 * u  v  actual_ang   req_ang    atan(...)
 * +  -    -ve          +ve         u/v
 * -  -    +ve          -ve         u/v
 * +  +    +ve         180-ang      u/v
 * -  +    -ve         180+ang      u/v
 */
  //cout<<"\nang:"<<ang<<endl;
  ang*=(-1);
  if(!usign && !vsign)
    ang+=180;
  if(usign && !vsign)
    ang-=180;
  //cout<<"\nang:"<<ang<<endl;
  {
    float ht = 0.9*(rty-lby) /max *mag;
    
    glPushMatrix();
    glTranslatef ((lbx+rtx)/2, (rty+lby)/2, 0.0);
    glRotatef(ang,0.0,0.0,1.0);
    glTranslatef (-(lbx+rtx)/2, -((rty+lby)/2), 0.0);
    
    //cout<<" "<<max<<" "<<mag<<endl;
    glColor3f(1.0,0,0);
    glBegin(GL_LINES);
    glVertex2f((lbx+rtx)/2, (rty+lby)/2+ht/2);
    glVertex2f((lbx+rtx)/2, (rty+lby)/2-ht/2);
    
    glVertex2f((lbx+rtx)/2, (rty+lby)/2-ht/2);
    glVertex2f((lbx+rtx)/2+ht/10, (rty+lby)/2-ht/2+ht/10);
    
    glVertex2f((lbx+rtx)/2, (rty+lby)/2-ht/2);
    glVertex2f((lbx+rtx)/2-ht/10, (rty+lby)/2-ht/2+ht/10);
    glEnd();
    glPopMatrix();
  }
}


double Renderer :: getMax(matrix<double> mat) //get max value from the 2D array
{
  double max = 0.0;
  for(unsigned int i=1;i< mat.size1()-1;i++)
    for(unsigned int j=1;j<mat.size2()-1;j++)
      if(max<mat(i,j))
	max=mat(i,j);
  return max;
}



void Renderer :: renderDensity(matrix<double> mat)
{
  int d00,d01,d10,d11;
  double gridLBX = this->gridLBX;
  double gridLBY = this->gridLBY;
  for (unsigned int i=0;i< mat.size1()-1;i++)
    {
      for (unsigned int j=0;j < mat.size2()-1;j++)
	{
	  glBegin ( GL_QUADS );
	  d00 = mat(i,j);
	  d01 = mat(i,j+1);
	  d10 = mat(i+1,j);
	  d11 = mat(i+1,j+1);
	  
	  glColor3f ( d00, d00, d00 ); glVertex2f ( gridLBX, gridLBY );
	  glColor3f ( d10, d10, d10 ); glVertex2f ( gridLBX, gridLBY+stepY );
	  glColor3f ( d11, d11, d11 ); glVertex2f ( gridLBX+stepX, gridLBY+stepY );
	  glColor3f ( d01, d01, d01 ); glVertex2f (  gridLBX+stepX, gridLBY );
	  gridLBX+=stepX;
	  glEnd();
	}
      gridLBY+=stepY;
      gridLBX = this->gridLBX;
    }
}


/*---for staggered with stam---*/

void Renderer :: renderVel2D_Stam()
{
  double h;
  //h = 1.0f;// /sGrid->nX ;
  h = sGrid->dx;
  double x,y ;
  matrix <double> u = sGrid->u;
  matrix <double> v = sGrid->v;
  glColor3f( 1.0f, 1.0f, 1.0f);
  glLineWidth ( 1.0f );
  glBegin (GL_LINES) ;
  for( int i=1; i< sGrid->nY; i++){  //not showing boundary..
    y = (i+0.5f)*h ;
    
    for( int j=1; j< sGrid->nX; j++){
      x = (j+0.5f)*h;
      
      glVertex2f( x, y );
      glVertex2f( x+U(i,j),  y+V(i,j) ) ;
      
    }
  }
  glEnd();
}

void Renderer :: renderDen2D_Stam()
{
  int i, j;
  double x, y, h, d00, d01, d10, d11;
  
  //h = 1.0f;// /sGrid->nX;
  h = sGrid->dx;
  
  glBegin ( GL_QUADS );
  /* here last grid row and col is not rendered..*/
  for ( i=0 ; i< sGrid->nY - 1 ; i++ ) { //ignored the right/top boundary
    y = (i+0.5f)*h;
    for ( j=0 ; j < sGrid->nX - 1 ; j++ ) {
      x = (j+0.5f)*h; // this 0.5 matter is difficult to understand..if removed smoothness reduces..
      d00 = sGrid->p(i,j);
      d01 = sGrid->p(i,j+1);
      d10 = sGrid->p(i+1,j);
      d11 = sGrid->p(i+1,j+1);
      
      glColor3f ( d00, d00, d00 ); glVertex2f ( x, y );
      glColor3f ( d10, d10, d10 ); glVertex2f ( x, y+h );
      glColor3f ( d11, d11, d11 ); glVertex2f ( x+h, y+h );
      glColor3f ( d01, d01, d01 ); glVertex2f (  x+h, y );
    }
  }
  glEnd ();
}


void Renderer :: renderParticles()
{
  int a;
  // std::cin>>a;
  //cout<<"renderPar"<<endl;
  glColor3f(0.3,0.3,0.9);
  //renderGrid();
  //glColor3f(0.2,0.2,0.9);
  //renderBoundary();
#ifdef DL
  // GLfloat *vertices	;
   // vertices = new GLfloat[sGrid->fluidParticles.size()*2];
  //float vertices[GRID_SIZE*GRID_SIZE*31];
  GLfloat *vertices= new GLfloat[sGrid->fluidParticles.size()*2];
  GLfloat *colors= new GLfloat[sGrid->fluidParticles.size()*3];
  int count=0,k=0;
#pragma omp parallel for	
  for (unsigned i = 0; i < sGrid->fluidParticles.size() ; i++ ){
      vertices[2*i] =   sGrid->fluidParticles.at(i)->x ;
      vertices[2*i+1] =   sGrid->fluidParticles.at(i)->y ;
  
      colors[k++] = 0.0;
      colors[k++] = 0.0;
      colors[k++] = 0.6;

      count++;
  }
  /////////////////////////
  
 #endif
  
#ifndef DL
  // GLfloat *vertices	;
  // vertices = new GLfloat[sGrid->fluidParticles.size()*2];
  float *colors= new float[sGrid->fluidParticles.size()*3];
  float *vertices= new float[sGrid->fluidParticles.size()*2];
  //float vertices[GRID_SIZE*GRID_SIZE*31];
  for (unsigned i = 0; i < sGrid->fluidParticles.size() ; i++ ){
    /* glPointSize(4);
       glBegin(GL_POINTS);
       //glColor3f(1,i*0.001,0);
       glColor3f(0,0,0.6);
       glVertex2d(sGrid->fluidParticles.at(i)->x,sGrid->fluidParticles.at(i)->y);
       glEnd();*/
    vertices[2*i]=sGrid->fluidParticles.at(i)->x;
    vertices[2*i+1]=sGrid->fluidParticles.at(i)->y;
 
    colors[k++]=0.0; 
    colors[k++]=0.0; 
    colors[k++]=0.6;
    count++;
 
  }
#endif
  ///////////////////////
    ///////////
  //  glColor3f(0,0,0.6);
  glPointSize(4);
    
  // Create a Vector Buffer Object that will store the vertices on video memory
  GLuint vbo,vao;
  GLuint colorbuffer;
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 2*count*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  
  glBindVertexArray (vao);
  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, (k)*sizeof(GLfloat), colors, GL_STATIC_DRAW);
  
  glBindVertexArray(vao);

  //Enable the vertex attribute
   glEnableVertexAttribArray (0);
   glEnableClientState(GL_VERTEX_ARRAY);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
   
   glEnableVertexAttribArray(1);
   glEnableClientState(GL_VERTEX_ARRAY);
   glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
   glDrawArrays(GL_POINTS,0, sGrid->fluidParticles.size());
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glBindVertexArray(0);

   delete colors;
   delete vertices;
  ////////////////////////

}

void Renderer :: renderParticle(double x, double y,double R,double G,double B,int psize)
{
  
  glPointSize(psize);
  glBegin(GL_POINTS);
  glColor3f(R,G,B);
  glVertex2d(x,y);
  glEnd();
}

void Renderer :: renderSurfaceBoundary() {
 
  unsigned no=0;
  float *vertices=new float[sGrid->nX*sGrid->nY*2];
   
  for (int i = sGrid->nX - 1; i > 0; i--)
    for (int j = 0; j < sGrid->nY-1; j++) {
      int state=0;
      bool UL = sGrid->cellType(i, j) == FLUID,
	UR = sGrid->cellType(i, j+1) == FLUID,
	BL = sGrid->cellType(i-1, j) == FLUID,
	BR = sGrid->cellType(i-1 ,j+1) == FLUID ;
      
      if (UL)
	state = state | 1;
      if (UR)
	state = state | 2;
      if (BL)
	state = state | 4;
      if (BR)
	state = state | 8;
      
      if(!state)
	continue;
      
      double x = gridLBX + (j) * stepX;
      double y = gridLBY + (i) * stepY;
      double incfactor = stepX / 2.0;
      x+=incfactor;
      y+=incfactor;
      glColor3f(1.0, 0.0, 0.0);
      switch (state) {
      case 1:
	// glVertex2f(x, y - incfactor);
	//  glVertex2f(x + incfactor, y);
	vertices[no++]=x;        
	vertices[no++]=y-incfactor;
	vertices[no++]=x+incfactor;
	vertices[no++]=y;
	break;
	
      case 2:
	//glVertex2f(x + incfactor, y);
	//glVertex2f(x + 2 * incfactor, y - incfactor);
	vertices[no++]=x+incfactor; 
	vertices[no++]=y;
	vertices[no++]=x=2*incfactor;
	vertices[no++]=y-incfactor;
	break;
	
      case 3:
	//  glVertex2f(x, y - incfactor);
	//  glVertex2f(x + 2 * incfactor, y - incfactor);
	vertices[no++]=x;             
	vertices[no++]=y-incfactor;
	vertices[no++]=x+2*incfactor;  
	vertices[no++]=y-incfactor;
	break;
		      
      case 4:
	// glVertex2f(x, y - incfactor);
	// glVertex2f(x + incfactor, y - 2 * incfactor);
	vertices[no++]=x;     
	vertices[no++]=y-incfactor;
	vertices[no++]=x+incfactor; 
	vertices[no++]=y-2*incfactor;
	break;

      case 5:
	// glVertex2f(x + incfactor, y);
	// glVertex2f(x + incfactor, y - 2 * incfactor);
	vertices[no++]=x+incfactor;  
	vertices[no++]=y;
	vertices[no++]=x+incfactor; 
	vertices[no++]=y-2*incfactor;
	break;
	
      case 6:
	// glVertex2f(x, y - incfactor);
	// glVertex2f(x + incfactor, y);
	// glVertex2f(x + incfactor, y - 2 * incfactor);
	// glVertex2f(x + 2 * incfactor, y - incfactor);
	vertices[no++]=x; 
	vertices[no++]=y-incfactor;
	vertices[no++]=x+incfactor; 
	vertices[no++]=y;
	vertices[no++]=x+incfactor; 
	vertices[no++]=y-2*incfactor;
	vertices[no++]=x+2*incfactor;
	vertices[no++]=y-incfactor;
	break;
	
      case 7:
	// glVertex2f(x + incfactor, y - 2 * incfactor);
	// glVertex2f(x + 2 * incfactor, y - incfactor);
	vertices[no++]=x+incfactor; 
	vertices[no++]=y-2*incfactor;
	vertices[no++]=x+2*incfactor;
	vertices[no++]=y-incfactor;
	break;

      case 8:
	// glVertex2f(x + incfactor, y - 2 * incfactor);
	// glVertex2f(x + 2 * incfactor, y - incfactor);
	vertices[no++]=x+incfactor;  
	vertices[no++]=y-2*incfactor;
	vertices[no++]=x+2*incfactor; 
	vertices[no++]=y-incfactor;
	break;
		      
      case 9:
	// glVertex2f(x + incfactor, y);
	// glVertex2f(x + 2 * incfactor, y - incfactor);
	// glVertex2f(x, y - incfactor);
	// glVertex2f(x + incfactor, y - 2 * incfactor);
	vertices[no++]=x+incfactor; 
	vertices[no++]=y;
	vertices[no++]=x+2*incfactor; 
	vertices[no++]=y-incfactor;
	vertices[no++]=x;  
	vertices[no++]=y-incfactor;
	vertices[no++]=x+incfactor; 
	vertices[no++]=y-2*incfactor;
	break;

      case 10:
	// glVertex2f(x + incfactor, y);
	// glVertex2f(x + incfactor, y - 2 * incfactor);
	vertices[no++]=x+incfactor;  
	vertices[no++]=y;
	vertices[no++]=x+incfactor; 
	vertices[no++]=y-2*incfactor;
	break;

      case 11:
	// glVertex2f(x, y - incfactor);
	// glVertex2f(x + incfactor, y - 2 * incfactor);
	vertices[no++]=x; 
	vertices[no++]=y-incfactor;
	vertices[no++]=x+incfactor; 
	vertices[no++]=y-2*incfactor;
	break;

      case 12:
	// glVertex2f(x, y - incfactor);
	// glVertex2f(x + 2 * incfactor, y - incfactor);
	vertices[no++]=x;  
	vertices[no++]=y-incfactor;
	vertices[no++]=x+2*incfactor;
	vertices[no++]=y-incfactor;
	break;
	
      case 13:
	// glVertex2f(x + incfactor, y);
	// glVertex2f(x + 2 * incfactor, y - incfactor);
	vertices[no++]=x+incfactor;
	vertices[no++]=y;
	vertices[no++]=x+2*incfactor; 
	vertices[no++]=y-incfactor;
	break;
		      
      case 14:
	// glVertex2f(x, y - incfactor);
	// glVertex2f(x + incfactor, y);
	vertices[no++]=x; 
	vertices[no++]=y-incfactor;
	vertices[no++]=x+incfactor;
	vertices[no++]=y;
	break;
      }
      
    }
  
  glColor3f(0,0,0.6);
  glPointSize(4);
  GLuint vbo,vao;
  
  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (3, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer - 9 float values, start pointer and static data
  glBufferData (GL_ARRAY_BUFFER, no*sizeof (float), vertices, GL_STATIC_DRAW);
  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (3, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);
  //Enable the vertex attribute
  glEnableVertexAttribArray (0);
  //This the layout of our first vertex buffer
  //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats 
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  glDrawArrays(GL_LINES, 0, no/2);
  glBindVertexArray(0);
  glDisableClientState(GL_VERTEX_ARRAY);
  

  ////////////////////////////////////////////////////////////////
      delete vertices; 
 
  
// */
   ////////////////////////////////////////////////////////////////
}



