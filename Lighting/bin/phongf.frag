#version 410
in vec4 vColour; 
in float vtime;
out vec4 fragColor; 
void main() { 
	
	vec4 c = vec4(vColour.x * abs(cos(vtime)), vColour.y * abs(sin(vtime)), vColour.z * abs(tan(vtime)), 1);	
	fragColor = c; 
}