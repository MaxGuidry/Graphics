#include "CameraApp.h"
#include <gl_core_4_4.h>
#include<glfw/glfw3.h>
#include "Gizmos.h"
#include<glm/glm.hpp>

glm::mat4 mercury;
glm::mat4 venus;
glm::mat4 earth;
glm::mat4 mars;
glm::mat4 jupiter;
glm::mat4 saturn;
glm::mat4 uranus;
glm::mat4 neptune;
glm::mat4 moon;
CameraApp::CameraApp() : m_camera(new DollyCamera())
{
}


CameraApp::~CameraApp()
{
}

bool CameraApp::Update(float deltaTime)
{
	static bool mouse_down = false;
	static double mousex = 0;
	static double mousey = 0;

	if (mouse_down == false)
	{
		mouse_down = true;
		glfwGetCursorPos(window, &mousex, &mousey);
	}

	glfwGetCursorPos(window, &mousex, &mousey);
	glm::vec2 cpos = glm::vec2(mousex, mousey);

	glm::vec2 deltaMouse = glm::vec2(mousex - pmouseX, mousey - pmouseY);
	deltaMouse = deltaMouse * deltaTime * 15.f;

	if (glfwGetMouseButton(window, 0))
		m_camera->LookAround(deltaMouse);

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		glm::vec3 right = glm::vec3(m_camera->m_view[0][0], m_camera->m_view[1][0], m_camera->m_view[2][0]);
		m_camera->setPosition(right);
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		glm::vec3 forward = glm::vec3(m_camera->m_view[0][2], m_camera->m_view[1][2], m_camera->m_view[2][2]);
		m_camera->setPosition(-forward);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		glm::vec3 forward = glm::vec3(m_camera->m_view[0][2], m_camera->m_view[1][2], m_camera->m_view[2][2]);
		m_camera->setPosition(forward);
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		glm::vec3 right = glm::vec3(m_camera->m_view[0][0], m_camera->m_view[1][0], m_camera->m_view[2][0]);
		m_camera->setPosition(-right);
	}
	glm::vec3 mercpos = m_camera->RotateAround(glm::vec3(mercury[3][0], mercury[3][1], mercury[3][2]), glm::vec3(0), glm::vec2(.1f, 0), mercury);
	mercury[3][0] = mercpos.x;
	mercury[3][1] = mercpos.y;
	mercury[3][2] = mercpos.z;
	glm::vec3 venuspos = m_camera->RotateAround(glm::vec3(venus[3][0], venus[3][1], venus[3][2]), glm::vec3(0), glm::vec2(.075f, 0), venus);
	venus[3][0] = venuspos.x;
	venus[3][1] = venuspos.y;
	venus[3][2] = venuspos.z;
	glm::vec3 earthpos = m_camera->RotateAround(glm::vec3(earth[3][0], earth[3][1], earth[3][2]), glm::vec3(0), glm::vec2(.06f, 0), earth);
	earth[3][0] = earthpos.x;
	earth[3][1] = earthpos.y;
	earth[3][2] = earthpos.z;
	glm::vec3 marspos = m_camera->RotateAround(glm::vec3(mars[3][0], mars[3][1], mars[3][2]), glm::vec3(0), glm::vec2(.05f, 0), mars);
	mars[3][0] = marspos.x;
	mars[3][1] = marspos.y;
	mars[3][2] = marspos.z;
	glm::vec3 jupiterpos = m_camera->RotateAround(glm::vec3(jupiter[3][0], jupiter[3][1], jupiter[3][2]), glm::vec3(0), glm::vec2(.04f, 0), jupiter);
	jupiter[3][0] = jupiterpos.x;
	jupiter[3][1] = jupiterpos.y;
	jupiter[3][2] = jupiterpos.z;
	glm::vec3 saturnpos = m_camera->RotateAround(glm::vec3(saturn[3][0], saturn[3][1], saturn[3][2]), glm::vec3(0), glm::vec2(.025f, 0), saturn);
	saturn[3][0] = saturnpos.x;
	saturn[3][1] = saturnpos.y;
	saturn[3][2] = saturnpos.z;
	glm::vec3 uranuspos = m_camera->RotateAround(glm::vec3(uranus[3][0], uranus[3][1], uranus[3][2]), glm::vec3(0), glm::vec2(.015f, 0), uranus);
	uranus[3][0] = uranuspos.x;
	uranus[3][1] = uranuspos.y;
	uranus[3][2] = uranuspos.z;
	glm::vec3 neppos = m_camera->RotateAround(glm::vec3(neptune[3][0], neptune[3][1], neptune[3][2]), glm::vec3(0), glm::vec2(.005f, 0), neptune);
	neptune[3][0] = neppos.x;
	neptune[3][1] = neppos.y;
	neptune[3][2] = neppos.z;
	glm::vec3 moonpos = m_camera->RotateAround(glm::vec3(moon[3][0], moon[3][1], moon[3][2]), glm::vec3(earth[3][0], earth[3][1], earth[3][2]), glm::vec2(.5f, 0), moon);
	moon[3][0] = moonpos.x;
	moon[3][1] = moonpos.y;
	moon[3][2] = moonpos.z;
	pmouseX = mousex;
	pmouseY = mousey;

	return true;
}
bool CameraApp::Draw()
{

	/*glUseProgram(shader_programme);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,3);
	*/


	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1));
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(
			glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(
			glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i),
			i == 10 ? white : black);

	}
	Gizmos::addSphere(glm::vec3(mercury[3][0], mercury[3][1], mercury[3][2]), .3f, 10, 10, glm::vec4(0, .4, .7, 1), &mercury);
	Gizmos::addSphere(glm::vec3(venus[3][0], venus[3][1], venus[3][2]), .3f, 10, 10, glm::vec4(0, .4, .7, 1), &venus);
	Gizmos::addSphere(glm::vec3(earth[3][0], earth[3][1], earth[3][2]), .3f, 10, 10, glm::vec4(0, .4, .7, 1), &earth);
	Gizmos::addSphere(glm::vec3(mars[3][0], mars[3][1], mars[3][2]), .3f, 10, 10, glm::vec4(0, .4, .7, 1), &mars);
	Gizmos::addSphere(glm::vec3(jupiter[3][0], jupiter[3][1], jupiter[3][2]), .3f, 10, 10, glm::vec4(0, .4, .7, 1), &jupiter);
	Gizmos::addSphere(glm::vec3(saturn[3][0], saturn[3][1], saturn[3][2]), .3f, 10, 10, glm::vec4(0, .4, .7, 1), &saturn);
	Gizmos::addSphere(glm::vec3(uranus[3][0], uranus[3][1], uranus[3][2]), .3f, 10, 10, glm::vec4(0, .4, .7, 1), &uranus);
	Gizmos::addSphere(glm::vec3(neptune[3][0], neptune[3][1], neptune[3][2]), .3f, 10, 10, glm::vec4(0, .4, .7, 1), &neptune);
	Gizmos::addSphere(glm::vec3(moon[3][0], moon[3][1], moon[3][2]), .1f, 10, 10, glm::vec4(0, .4, .7, 1), &moon);
	Gizmos::addSphere(glm::vec3(0, 0, 0), 1.f, 50, 50, black);
	Gizmos::draw(m_camera->getProjectionView());
	return true;
}
bool CameraApp::Start()
{
	this->m_camera->LookAt(m_camera->m_position, glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));
	mercury = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		2, 0, 2, 1
	);
	venus = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3, 0, 3, 1
	);
	earth = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		4, 0, 4, 1
	);
	mars = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		5, 0, 5, 1
	);
	jupiter = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		6, 0, 6, 1
	);
	saturn = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		7, 0, 7, 1
	);
	uranus = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		8, 0, 8, 1
	);
	neptune = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		9, 0, 9, 1
	);
	moon = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		earth[3][0] + .6f, 0, earth[3][2], 1
	);
	/*float points[] = {
	0.0f,  0.5f,  0.0f,
	0.5f, -0.5f,  0.0f,
	-0.5f, -0.5f,  0.0f
	};
	GLuint vbo = 0;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	const char* vertex_shader =
	"#version 400\n"
	"in vec3 vp;"
	"void main() {"
	"  gl_Position = vec4(vp, 1.0);"
	"}";
	const char* fragment_shader =
	"#version 400\n"
	"out vec4 frag_colour;"
	"void main() {"
	"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
	"}";
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);*/
	Gizmos::create();

	return true;
}

bool CameraApp::Shutdown()
{
	return true;
}






