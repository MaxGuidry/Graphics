#include "Mesh.h"
#include <gl_core_4_4.h>
#include "gl_core_4_4.h"


Mesh::Mesh() : index_Count(0), vertex_Count(0), m_vao(0), m_vbo(0), m_ibo(0), m_tbo(0), vertRef(m_vertices), indicesRef(m_indices), vboptr(&m_vbo), vaoptr(&m_vbo), iboptr(&m_ibo)
{
}


Mesh::~Mesh()
{
}

void Mesh::create_buffers()
{
	glGenTextures(1, &m_tbo);
	glBindTexture(GL_TEXTURE_2D, m_tbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	
	
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices.front()), m_vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(m_indices.front()), m_indices.data(), GL_STATIC_DRAW);
	
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));


	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3 + sizeof(glm::vec2)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tbo);
	glBindVertexArray(m_vao);
	
	

}

void Mesh::unbind()
{
	glBindVertexArray(0);
}

bool Mesh::draw(GLint drawmode)
{
	switch (drawmode)
	{
	case GL_LINES:

		glDrawElements(GL_LINES, this->index_Count, GL_UNSIGNED_INT, 0);
		break;

	case GL_TRIANGLES:
		glDrawElements(GL_TRIANGLES, this->index_Count, GL_UNSIGNED_INT, 0);
		break;

	case GL_TRIANGLE_STRIP:
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		glDrawElements(GL_TRIANGLE_STRIP, this->index_Count, GL_UNSIGNED_INT, 0);
		glDisable(GL_PRIMITIVE_RESTART);
		break;

	case GL_POINTS:
		glDrawArrays(GL_POINTS, 0, this->vertRef.size());
		break;
	default:
		return false;
	}
	return true;
}

std::vector<Vertex> Mesh::getVerts()
{
	return this->m_vertices;
}

std::vector<unsigned> Mesh::getIndices()
{
	return this->m_indices;
}
