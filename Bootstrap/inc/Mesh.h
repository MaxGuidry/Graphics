#pragma once
#include <vector>
#include<glm/glm.hpp>
#include "TestApp.h"


struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	glm::vec2 texcoord;
	glm::vec4 tangent;
	glm::vec4 bitangent;
};
struct Texture
{
	int width, height, format;
	unsigned char * data;
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
	bool draw(GLint drawmode);
	unsigned int index_Count;
	unsigned int vertex_Count;
	const std::vector<Vertex>& vertRef;
	const std::vector<unsigned int>& indicesRef;
	std::vector<Vertex>getVerts();
	std::vector<unsigned int>getIndices();
	unsigned int * vaoptr;
	unsigned int * vboptr;
	unsigned int * iboptr;
	glm::vec3 m_position;
	Texture texture;
	
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned int m_tbo;
	std::vector<unsigned int> m_indices;
	std::vector<Vertex> m_vertices;
};

