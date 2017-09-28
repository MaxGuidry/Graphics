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


Shader vert;
Shader frag;
GLuint shader;
TextureApp::TextureApp() : GridMesh(), cam(),textureName(new char)
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

	cam.LookAt(cam.m_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GridMesh = GenGrid(3, 3);
	shader = glCreateProgram();
	vert = Shader(shader);
	vert.load("vertex.vert", GL_VERTEX_SHADER);
	vert.attach();
	frag = Shader(shader);
	frag.load("fragment.frag", GL_FRAGMENT_SHADER);
	frag.attach();

	//GridMesh->loadTexture("./space.png", STBI_default);
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
		GridMesh->loadTexture(textureName, STBI_default);
	glfwGetCursorPos(window, &mousex, &mousey);
	glm::vec2 cpos = glm::vec2(mousex, mousey);

	glm::vec2 deltaMouse = glm::vec2(mousex - pmouseX, mousey - pmouseY);
	deltaMouse = deltaMouse * deltaTime;

	if (glfwGetMouseButton(window, 0))
		cam.LookAround(deltaMouse);

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		glm::vec3 right = glm::vec3(cam.m_view[0][0], cam.m_view[1][0], cam.m_view[2][0]);
		cam.setPosition(right * .25f);
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		glm::vec3 forward = glm::vec3(cam.m_view[0][2], cam.m_view[1][2], cam.m_view[2][2]);
		cam.setPosition(-forward* .25f);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		glm::vec3 forward = glm::vec3(cam.m_view[0][2], cam.m_view[1][2], cam.m_view[2][2]);
		cam.setPosition(forward* .25f);
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		glm::vec3 right = glm::vec3(cam.m_view[0][0], cam.m_view[1][0], cam.m_view[2][0]);
		cam.setPosition(-right* .25f);
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
	
	GridMesh->bind();

	GridMesh->draw(GL_TRIANGLES);
	GridMesh->unbind();
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
			Vertex v = { glm::vec4(float(c), 0, float(r), 1),glm::vec4(sin(r),cos(c),0,1),glm::vec4(0,1,0,0),glm::vec2(float(c)  / float(cols - 1),float(r) / float(rows-1)) };
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
