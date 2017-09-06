#pragma once
#include "Application.h"
#include "DollyCamera.h"
#include "Mesh.h"

class RenderingApp :
	public Application
{
public:
	
	RenderingApp();
	~RenderingApp();
	Mesh * GenGrid(unsigned int rows, unsigned int cols);
	Mesh * GenSphere(float radius, unsigned int verts);
	unsigned int m_StandardShaderID;
	DollyCamera * camera;
	Mesh * mesh;
protected:
	bool Start() override;
	bool Update(float deltaTime) override;
	bool Shutdown() override;
	bool Draw() override;
};