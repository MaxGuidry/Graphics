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
vec3 L;
void main()
{
	//float lambertTerm = max(0, dot(direction, vec3(vNormal)));	
	//vec3 reflectedRay = reflect(direction, vec3(vNormal));
	//vec3 E = normalize(camforward - vec3(vPosition));
	//vec3 halfway = normalize(direction + E);
	//float DotRV = max(dot(reflectedRay, -E),0.0f);
	//float DotRV = abs(min(dot(halfway, vec3(vNormal)),0.0f));
	//float DotRV = max(dot(halfway, vec3(vNormal)),0.0f);
	//float rvToa = pow(DotRV, a);
	//FragColor = vec4((Ka*Ia) +(Id*Kd*lambertTerm)+ (Ks * Is * rvToa ),1);

	vec3 N = normalize(vNormal.xyz);
    vec3 P = vPosition.xyz;
    L = normalize(direction);
    vec3 Ambient = Ka * Ia;
    float Ndl = max(0.0f,dot(L, N));
    vec3 Diffuse = Kd * Id * Ndl;
    vec3 R = reflect(L, N);
    vec3 E = normalize(camforward-P);
    float specTerm = pow(max(0.0f, dot(R, E)), a);
    vec3 Specular = Ks * Is * specTerm;
    FragColor = vec4(Ambient + Diffuse + Specular ,1);
}