#include "TextureApp.h"
#include "gl_core_4_4.h"
#include<glfw/include/GLFW/glfw3.h>
#include "Shader.h"

#include<glm/ext.hpp>
#include<glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "MaxGizmos.h"

void generateSphere(unsigned int segments, unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount);
unsigned int vao;
unsigned int vbo;
unsigned int ibo;
unsigned int indexcount;
Shader vert;
Shader frag;
GLuint shader;
TextureApp::TextureApp() : GridMesh(), cam(), textureName(new char)
{
}


TextureApp::~TextureApp()
{
}

bool TextureApp::Start()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImGui::CreateContext();
	ImGui::GetIO().DisplaySize = ImVec2(20, 20);
	ImGui_ImplGlfwGL3_Init(window, true);
	m_directLight.diffuse = glm::vec3(1);
	m_directLight.specular = glm::vec3(1);
	m_ambientLight = glm::vec3(1);

	m_material.diffuse = glm::vec3(1);
	m_material.ambient = glm::vec3(1);
	m_material.specular = glm::vec3(1);

	m_material.specularPower = 30;
	m_directLight.direction = glm::vec3(1, 1, 1);
	cam.LookAt(cam.m_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GridMesh = GenGrid(3, 3);
	shader = glCreateProgram();
	vert = Shader(shader);
	vert.load("vertex.vert", GL_VERTEX_SHADER);
	vert.attach();
	frag = Shader(shader);
	frag.load("fragment.frag", GL_FRAGMENT_SHADER);
	frag.attach();
	generateSphere(70, 70, vao, vbo, ibo, indexcount);
	//GridMesh->loadTexture("./space.png", STBI_default);
	Mesh m = MaxGizmos::GenSphere(1, 50, 50);
	m.create_buffers();
	//m.m_position = glm::vec3(objects.size(), objects.size(), objects.size());
	m.loadTexture("./space.png", STBI_rgb_alpha);
	m.create_buffers();
	objects.push_back(m);


	GridMesh->create_buffers();

	return true;
}
static double mousex = 0;
static double mousey = 0;
static double pmouseX = 0;
static double pmouseY = 0;
static bool F1 = false;
static bool F5 = false;
bool TextureApp::Update(float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_F12))
		objects[0].loadTexture(textureName, STBI_default);
	glfwGetCursorPos(window, &mousex, &mousey);
	glm::vec2 cpos = glm::vec2(mousex, mousey);

	glm::vec2 deltaMouse = glm::vec2(mousex - pmouseX, mousey - pmouseY);
	deltaMouse = deltaMouse * deltaTime;

	if (glfwGetMouseButton(window, 0))
		cam.LookAround(deltaMouse);

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		glm::vec3 right = glm::vec3(cam.m_view[0][0], cam.m_view[1][0], cam.m_view[2][0]);
		cam.setPosition(right * .05f);
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		glm::vec3 forward = glm::vec3(cam.m_view[0][2], cam.m_view[1][2], cam.m_view[2][2]);
		cam.setPosition(-forward* .05f);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		glm::vec3 forward = glm::vec3(cam.m_view[0][2], cam.m_view[1][2], cam.m_view[2][2]);
		cam.setPosition(forward* .05f);
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		glm::vec3 right = glm::vec3(cam.m_view[0][0], cam.m_view[1][0], cam.m_view[2][0]);
		cam.setPosition(-right* .05f);
	}


	pmouseX = mousex;
	pmouseY = mousey;
	return true;
}

bool TextureApp::Shutdown()
{
	return true;
}

bool TextureApp::Draw()
{
	glUseProgram(shader);
	ImGui_ImplGlfwGL3_NewFrame();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	unsigned int pvm = vert.getUniform("ProjectionViewModel");
	glUniformMatrix4fv(pvm, 1, false, glm::value_ptr(cam.getProjectionView()));
	unsigned int texID = frag.getUniform("tex");
	glUniform1i(texID, 0);
	ImGui::Begin("test");
	ImGui::SetWindowSize(ImVec2(200, 200));
	ImGui::InputText("Texture File", textureName, 50);

	glUniformMatrix4fv(pvm, 1, false, glm::value_ptr(cam.getProjectionView() * glm::scale(glm::vec3(5))));
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

	lightUniform = frag.getUniform("Kd");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_material.diffuse));

	lightUniform = frag.getUniform("Is");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_directLight.specular));

	lightUniform = frag.getUniform("a");
	glUniform1f(lightUniform, m_material.specularPower);

	lightUniform = frag.getUniform("camforward");
	glUniform3fv(lightUniform, 1, glm::value_ptr(cam.m_position));

	/*GridMesh->bind();
	GridMesh->draw(GL_TRIANGLES);
	GridMesh->unbind();*/
	/*glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);*/
	for (auto m : objects)
	{

		unsigned int pvm = vert.getUniform("ProjectionViewModel");
		glUniformMatrix4fv(pvm, 1, false, glm::value_ptr(cam.getProjectionView() * glm::scale(glm::vec3(5)) *glm::translate(m.m_position)));


		m.bind();
		m.draw(GL_TRIANGLE_STRIP);
		m.unbind();
	}

	glUseProgram(0);
	ImGui::End();
	ImGui::Render();
	return true;
}
Mesh * TextureApp::GenGrid(unsigned int rows, unsigned int cols)
{
	Vertex * aoVerts = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
		{
			Vertex v = { glm::vec4(float(c), 0, float(r), 1),glm::vec4(sin(r),cos(c),0,1),glm::vec4(0,1,0,0),glm::vec2(float(c) / float(cols - 1),float(r) / float(rows - 1)) };
			//v.color = glm::vec4(1);
			aoVerts[r * cols + c] = v;
			//glm::vec3 color = glm::vec3(sinf( 12 * cosf(10 * tanf(float(c) / (float(cols))))), cosf(9 * sinf(21 * tanf(float(c) / (float(cols))))), tanf(34 * cosf(24 * sinf(float(c) / (float(cols))))));
			//glm::vec3 color = glm::vec3((float)r / (float)rows, ((float)r / (float)rows) * ((float)c / (float)cols), (float)c / (float)cols);
			//aoVerts[r * cols + c].color = glm::vec4(color, 1);
		}
	unsigned int * auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	unsigned int index = 0;
	std::vector<Vertex> verts = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	for (unsigned int r = 0; r < (rows - 1); ++r)
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	for (unsigned int i = 0; i < (rows * cols); i++)
		verts.push_back(aoVerts[i]);
	for (unsigned int i = 0; i < index; i++)
		indices.push_back(auiIndices[i]);
	Mesh * gridMesh = new Mesh();
	gridMesh->initialize(verts, indices);
	verts.clear();
	indices.clear();
	delete[] auiIndices;
	delete[] aoVerts;
	return gridMesh;
}



void generateSphere(unsigned int segments, unsigned int rings,
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