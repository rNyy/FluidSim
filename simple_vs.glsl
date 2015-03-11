#version 400

layout(location = 0) in vec3 vp;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main () 
{
  gl_Position = vec4 (vp, 1.0);
  fragmentColor = vec3(vertexColor);
	
}