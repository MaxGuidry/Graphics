#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include "DollyCamera.h"
#include "Mesh.h"

class RenderingApp :
	public Application
{
public:
	
	RenderingApp();
	~RenderingApp();
	Mesh * GenGrid(unsigned int rows, unsigned int cols);
	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
	unsigned int m_programID;
	unsigned int indexCount;
	DollyCamera * camera;
	Mesh * mesh;
protected:
	bool Start() override;
	bool Update(float deltaTime) override;
	bool Shutdown() override;
	bool Draw() override;
};

