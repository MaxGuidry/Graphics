#include "LightingApp.h"
#include "gl_core_4_4.h"
#include <glm/ext.hpp>
#include <glfw/include/glfw/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <MaxGizmos.h>


LightingApp::LightingApp() : camera(new DollyCamera()), m_fill(false)
{
}


LightingApp::~LightingApp()
{
}

bool LightingApp::Start()
{
	camera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	
	Mesh tmp = MaxGizmos::GenSphere(1.f, 15, 15);
	m_sphere.initialize(tmp.getVerts(), tmp.getIndices());
	m_sphere.create_buffers();


	m_PhongShader = glCreateProgram();
	Shader vertex = Shader(m_PhongShader);
	vertex.load("./phongv.vert", GL_VERTEX_SHADER);
	vertex.attach();

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
	deltaMouse = deltaMouse * deltaTime * 3.f;

	if (glfwGetMouseButton(window, 0))
		camera->LookAround(deltaMouse);

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		glm::vec3 right = glm::vec3(camera->m_view[0][0], camera->m_view[1][0], camera->m_view[2][0]);
		camera->setPosition(right);
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		glm::vec3 forward = glm::vec3(camera->m_view[0][2], camera->m_view[1][2], camera->m_view[2][2]);
		camera->setPosition(-forward);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		glm::vec3 forward = glm::vec3(camera->m_view[0][2], camera->m_view[1][2], camera->m_view[2][2]);
		camera->setPosition(forward);
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		glm::vec3 right = glm::vec3(camera->m_view[0][0], camera->m_view[1][0], camera->m_view[2][0]);
		camera->setPosition(-right);
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
	unsigned int pvU = glGetUniformLocation(m_PhongShader, "projectionViewWorldMatrix");
	glUniformMatrix4fv(pvU, 1, false, glm::value_ptr(camera->getProjectionView()));
	m_sphere.draw(GL_TRIANGLE_STRIP);
	m_sphere.unbind();
	glUseProgram(0);

	return true;
}
