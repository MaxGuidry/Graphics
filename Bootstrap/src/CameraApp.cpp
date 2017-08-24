#include "CameraApp.h"
#include <gl_core_4_4.h>
#include<glfw/glfw3.h>
#include "Gizmos.h"
#include<glm/glm.hpp>


CameraApp::CameraApp() : m_camera(new DollyCamera())
{
}


CameraApp::~CameraApp()
{
}

bool CameraApp::Update(float deltaTime)
{
	static bool mouse_down = false;
	/*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{*/
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
	std::cout << "x: " << deltaMouse.x << " y: " << deltaMouse.y << std::endl << std::flush;


	if (glfwGetMouseButton(window, 0))
		m_camera->LookAround(deltaMouse);

	pmouseX = mousex;
	pmouseY = mousey;

	//}
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
	Gizmos::addSphere(glm::vec3(0, 0, 0), 5.f, 50, 50, black);
	Gizmos::draw(m_camera->getProjectionView());
	return true;
}
bool CameraApp::Start()
{
	this->m_camera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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




bool CameraApp::Run(unsigned int width, unsigned int height, const char* appname, bool fullscreen)
{

	if (appname == nullptr || width == 0 || height == 0)
		return false;
	if (!glfwInit())
		return false;
	GLFWmonitor * monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
	this->window = glfwCreateWindow(width, height, appname, monitor, nullptr);
	assert(this->window != nullptr);
	glfwMakeContextCurrent(this->window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}
	glEnable(GL_DEPTH_TEST);
	double deltaTime = 0;
	double currentTime = glfwGetTime();
	double prevTime = 0;
	glfwSwapInterval(0);
	glClearColor(.2f, .3f, .5f, 1);
	Start();
	while (this->m_running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (glfwWindowShouldClose(this->window))
			this->m_running = false;
		if (glfwGetKey(this->window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(this->window, true);
		glfwPollEvents();
		this->Update((float)deltaTime);
		prevTime = currentTime;
		currentTime = float(glfwGetTime());
		deltaTime = (currentTime - prevTime);

		this->Draw();
		glfwSwapBuffers(this->window);
	}
	glfwDestroyWindow(this->window);
	glfwTerminate();
	return true;

}

