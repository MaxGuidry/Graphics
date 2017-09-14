#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 colour;
uniform float time;
out vec4 vColour;
out float vtime;
uniform mat4 projectionViewWorldMatrix;
void main() 
{ 
	
	vColour =  colour;
	vtime = time;
	gl_Position = projectionViewWorldMatrix * position;
}