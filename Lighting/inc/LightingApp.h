#pragma once
#include "Application.h"
#include "DollyCamera.h"
#include <Mesh.h>
#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>
class LightingApp :
	public Application
{
public:	
	LightingApp();
	void generateSphere(unsigned int segments, unsigned int rings, unsigned int& vao, unsigned int& vbo, unsigned int& ibo, unsigned int& indexCount);
	~LightingApp();
protected:
	bool Start() override;
	bool Update(float deltaTime) override;
	bool Shutdown() override;
	bool Draw() override;
	DollyCamera * camera;
	bool m_fill;
	GLuint m_PhongShader;
	Mesh m_sphere;
	Mesh * m_selectedObject;
	std::vector<Mesh> objects;
	struct DirectionalLight
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} m_directLight;

	glm::vec3 m_ambientLight;

	struct Material
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float specularPower;
	} m_material;
};

