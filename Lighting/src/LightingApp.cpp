#include "LightingApp.h"
#include "gl_core_4_4.h"
#include <glm/ext.hpp>
#include <glfw/include/glfw/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <MaxGizmos.h>

Shader frag;
Shader vert;
unsigned int vao;
unsigned int vbo;
unsigned int ibo;
unsigned int indexcount;
LightingApp::LightingApp() : camera(new DollyCamera()), m_fill(false)
{
}


LightingApp::~LightingApp()
{
}

bool LightingApp::Start()
{
	camera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	
	Mesh tmp = MaxGizmos::GenSphere(1.f, 30, 30);
	m_sphere.initialize(tmp.getVerts(), tmp.getIndices());
	m_sphere.create_buffers();

	m_directLight.diffuse = glm::vec3(0,1,0);
	m_directLight.specular = glm::vec3(0,0,1);
	m_ambientLight = glm::vec3(.25f);

	m_material.diffuse = glm::vec3(1,0,0);
	m_material.ambient = glm::vec3(1,0,0);
	m_material.specular = glm::vec3(1,0,0);

	m_material.specularPower = 4;
	//generateSphere(32, 32, vao, vbo, ibo, indexcount);
	
	m_PhongShader = glCreateProgram();
	vert = Shader(m_PhongShader);
	vert.load("./phongv.vert", GL_VERTEX_SHADER);
	vert.attach();
	frag = Shader(m_PhongShader);
	frag.load("./phongf.frag", GL_FRAGMENT_SHADER);
	frag.attach();
	return true;
}
static double mousex = 0;
static double mousey = 0;
static double pmouseX = 0;
static double pmouseY = 0;
static bool F1 = false;

bool LightingApp::Update(float deltaTime)
{
	static bool mouse_down = false;

	if (mouse_down == false)
	{
		mouse_down = true;
		glfwGetCursorPos(window, &mousex, &mousey);
	}

	glfwGetCursorPos(window, &mousex, &mousey);
	glm::vec2 cpos = glm::vec2(mousex, mousey);

	glm::vec2 deltaMouse = glm::vec2(mousex - pmouseX, mousey - pmouseY);
	deltaMouse = deltaMouse * deltaTime * 1.f;

	if (glfwGetMouseButton(window, 0))
		camera->LookAround(deltaMouse);

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		glm::vec3 right = glm::vec3(camera->m_view[0][0], camera->m_view[1][0], camera->m_view[2][0]);
		camera->setPosition(right * .25f);
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		glm::vec3 forward = glm::vec3(camera->m_view[0][2], camera->m_view[1][2], camera->m_view[2][2]);
		camera->setPosition(-forward* .25f);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		glm::vec3 forward = glm::vec3(camera->m_view[0][2], camera->m_view[1][2], camera->m_view[2][2]);
		camera->setPosition(forward* .25f);
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		glm::vec3 right = glm::vec3(camera->m_view[0][0], camera->m_view[1][0], camera->m_view[2][0]);
		camera->setPosition(-right* .25f);
	}
	pmouseX = mousex;
	pmouseY = mousey;
	bool prevF1 = F1;

	if (glfwGetKey(window, GLFW_KEY_F1))
	{
		F1 = true;
	}
	else
		F1 = false;

	if (F1 == true && prevF1 != F1)
		m_fill = !m_fill;
	if (m_fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float time = glfwGetTime();
	m_directLight.direction = glm::normalize(glm::vec3(-10.f, -7.f, -10.f));
	//m_directLight.diffuse = glm::vec3(sin(time), sin(2.f*time), cos(time));
	//m_directLight.direction = glm::normalize(glm::vec3(sin(time), -5.f, cos(time)));
	return true;
}


bool LightingApp::Shutdown()
{
	return true;
}

bool LightingApp::Draw()
{

	glUseProgram(m_PhongShader);
	m_sphere.bind();
	unsigned int pvU = vert.getUniform("ProjectionViewModel");
	glUniformMatrix4fv(pvU, 1, false, glm::value_ptr(camera->getProjectionView()));
	int lightUniform = frag.getUniform("direction");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_directLight.direction));

	lightUniform = frag.getUniform("Id");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_directLight.diffuse));

	lightUniform = frag.getUniform("Ia");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_ambientLight));

	lightUniform = frag.getUniform("Ka");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_material.ambient));

	lightUniform = frag.getUniform("Ks");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_material.specular));

	lightUniform = frag.getUniform("Is");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_directLight.specular));

	lightUniform = frag.getUniform("a");
	glUniform1f(lightUniform, m_material.specularPower);

	lightUniform = frag.getUniform("camforward");
	//glUniform3fv(lightUniform, 1, glm::value_ptr(glm::vec3(camera->getView()[3][0], camera->getView()[3][1], camera->getView()[3][2])));
	glUniform3fv(lightUniform, 1, glm::value_ptr(camera->m_position));
	//glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
	m_sphere.draw(GL_TRIANGLE_STRIP);
	m_sphere.unbind();
	glUseProgram(0);

	return true;
}
void LightingApp::generateSphere(unsigned int segments, unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount)
{

	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	// using AIEVertex for now, but could be any struct as long as it has the correct elements
	Vertex* vertices = new Vertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	float ringAngle = glm::pi<float>() / (rings + 1);
	float segmentAngle = 2.0f * glm::pi<float>() / segments;

	Vertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring)
	{
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment, ++vertex)
		{
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = glm::vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);
			vertex->normal = glm::vec4(x0, y0, z0, 0);

			vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);

			// not a part of the AIEVertex, but this is how w generate bitangents
			vertex->bitangent = glm::vec4(glm::cross(glm::vec3(vertex->normal), glm::vec3(vertex->tangent)), 0);

			vertex->texcoord = glm::vec2(segment / (float)segments, ring / (float)(rings + 1));
		}
	}

	unsigned int index = 0;
	for (unsigned i = 0; i < (rings + 1); ++i)
	{
		for (unsigned j = 0; j < segments; ++j)
		{
			indices[index++] = i * (segments + 1) + j;
			indices[index++] = (i + 1) * (segments + 1) + j;
			indices[index++] = i * (segments + 1) + (j + 1);

			indices[index++] = (i + 1) * (segments + 1) + (j + 1);
			indices[index++] = i * (segments + 1) + (j + 1);
			indices[index++] = (i + 1) * (segments + 1) + j;
		}
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &vao);

	// all changes will apply to this handle
	glBindVertexArray(vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	// texcoords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	// tangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3 + sizeof(glm::vec2)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
	delete[] vertices;
}