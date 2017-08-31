#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 colour;
uniform float time;
out vec4 vColour;
uniform mat4 projectionViewWorldMatrix;
void main() 
{ 
	vec4 c = vec4(colour.x * sin(time), colour.y * cos(time), colour.z * sin(time), 1);	
	vColour = c;

	vec4 p = vec4(position.x * sin(time), position.y * cos(time), position.z * sin(time), 1);	
	
 
	gl_Position = projectionViewWorldMatrix * p;
}