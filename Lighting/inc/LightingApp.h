#pragma once
#include "Application.h"
#include "DollyCamera.h"
#include <Mesh.h>

class LightingApp :
	public Application
{
public:
	LightingApp();
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
};

