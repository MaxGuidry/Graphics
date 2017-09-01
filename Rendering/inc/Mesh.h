#pragma once
#include <vector>
#include<glm/glm.hpp>

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
};
class Mesh
{
public:
	Mesh();
	~Mesh();
	void create_buffers();
	void initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indeces);
	void bind();
	void unbind();
	unsigned int index_Count;
	unsigned int vertex_Count;
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	std::vector<unsigned int> m_indeces;
	std::vector<Vertex> m_vertices;
};

