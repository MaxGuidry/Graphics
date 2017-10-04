#version 410
layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec4 Normal;
layout (location = 3) in vec2 UV;
layout (location = 4) in vec4 Tangent;

uniform float time;
uniform mat4 ProjectionViewModel;
out vec4 vPosition;
out vec4 vColor;
out vec4 vNormal;
out vec2 vUV;
out vec4 vTangent;
uniform sampler2D tex;


void main()
{
	vec4 npos = normalize(Position);
	vec4 pos = vec4(vec3(Position.x + sin( Position.x * (mod(time,60) + 30)) * .05f,Position.y + cos( Position.y *(mod(time,60) + 30) )  * .05f,Position.z + sin( Position.z *cos(mod(time,60) + 30) ) * .05f),1);
	//vec4 pos = vec4(vec3(Position.x + sin(Position.x * time),Position.y + cos(Position.y * time),Position.z + sin(Position.z * time)),1);
	//vPosition = pos;
	vec4 vpos = Position;
	//vpos.x += texture(tex,UV);
	//vpos.y += texture(tex,UV);
	//vpos.z += texture(tex,UV);
	vpos.y += texture(tex,UV).r *2;	
	vColor = Color;
	vNormal = Normal;
	vUV = UV;
	vTangent = Tangent;
	
	gl_Position = ProjectionViewModel * vpos;
}