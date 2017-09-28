#pragma once
#include "Application.h"
#include "Mesh.h"

class TextureApp :
	public Application
{
public:
	TextureApp();
	~TextureApp();
protected:
	bool Start() override;
	bool Update(float deltaTime) override;
	bool Shutdown() override;
	bool Draw() override;
	Mesh * GenGrid(unsigned int rows, unsigned int cols);
	Mesh * GridMesh;
	Mesh * SphereMesh;
	char * textureName;
	DollyCamera cam;
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

