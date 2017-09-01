#include "Mesh.h"
#include <gl_core_4_4.h>
#include <glm/gtc/type_ptr.inl>


Mesh::Mesh(): index_Count(0), vertex_Count(0), m_vao(0), m_vbo(0), m_ibo(0)
{
}


Mesh::~Mesh()
{
}

void Mesh::create_buffers()
{
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenVertexArrays(1, &m_vao);
	unsigned int * indeces = new unsigned int[m_indeces.size()];
	std::vector<unsigned int> indexcpy = m_indeces;
	for(int i = 0;i<m_indeces.size();i++)
	{
		indeces[i] = indexcpy.front();
		indexcpy.pop_back();
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indeces.size() * sizeof(m_indeces.front()),indeces, GL_STATIC_DRAW);
	Vertex * verts = new Vertex[m_vertices.size()];
	memcpy((void*)verts, (void*)&m_vertices, m_vertices.size() * sizeof(Vertex));
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices.front()),verts , GL_STATIC_DRAW);
	
}

void Mesh::initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indeces)
{
	m_vertices = verts;
	vertex_Count = m_vertices.size();
	m_indeces = indeces;
	index_Count = m_indeces.size();
}

void Mesh::bind()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
