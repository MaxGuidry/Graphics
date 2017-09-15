// Phong equation
#version 410


uniform vec3 camforward;


uniform vec3 direction;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 Ia;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float a;

in vec4 vNormal;
in vec4 vPosition;
in vec4 vColor;
in vec2 vUV;
in vec4 vTangent;

out vec4 FragColor;

void main()
{
	float lambertTerm = max(0, dot(vec4(-direction, 1), vNormal));	
	vec3 reflectedRay = reflect(-direction, vec3(vNormal));
	vec3 E = normalize(camforward - vec3(vPosition));
	float DotRV = min(dot(reflectedRay, E),0.0f);
	float rvToa = pow(DotRV, a);
	FragColor = vec4(Ka,1)*vec4(Ia,1) + vec4(Id,1) * lambertTerm + vec4(Ks,1) * rvToa * vec4(Is,1);
}