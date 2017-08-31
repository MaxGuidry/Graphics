#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 colour;
uniform float time;
out vec4 vColour;
uniform mat4 projectionViewWorldMatrix;
void main() { 
vColour = colour;
vec4 c = vec4(sin(time),0,0,0);
 vColour = vColour + c;
 vColour = vec4(vColour.x / vColour.m,vColour.y / vColour.m,vColour.z / vColour.m,1);
 gl_Position = projectionViewWorldMatrix * position;
 }