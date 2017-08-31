#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include "DollyCamera.h"
class RenderingApp :
	public Application
{
public:
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 color;
	};
	RenderingApp();
	~RenderingApp();
	void GenGrid(unsigned int rows, unsigned int cols);
	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
	unsigned int m_programID;
	unsigned int indexCount;
	DollyCamera * camera;
protected:
	bool Start() override;
	bool Update(float deltaTime) override;
	bool Shutdown() override;
	bool Draw() override;
};

