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
	DollyCamera cam;
};

