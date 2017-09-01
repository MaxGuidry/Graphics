#include "Mesh.h"
#include <gl_core_4_4.h>
#include <glm/gtc/type_ptr.inl>


Mesh::Mesh() : index_Count(0), vertex_Count(0), m_vao(0), m_vbo(0), m_ibo(0)
{
}


Mesh::~Mesh()
{
}

void Mesh::create_buffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

}

void Mesh::initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indeces)
{
	for (auto v : verts)
		m_vertices.push_back(v);
	vertex_Count = m_vertices.size();
	for (auto i : indeces)
		m_indices.push_back(i);
	index_Count = m_indices.size();
}

void Mesh::bind()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices.front()), m_vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(m_indices.front()), m_indices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	
}

void Mesh::unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
