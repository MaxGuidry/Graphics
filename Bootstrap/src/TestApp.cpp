#include "TestApp.h"
#include <gl_core_4_4.h>
#include<glfw/glfw3.h>

TestApp::TestApp(): shader_programme(0)
{
}


TestApp::~TestApp()
{
}

bool TestApp::Update(float deltaTime)
{
	return true;
}
bool TestApp::Draw()
{
	
	glUseProgram(shader_programme);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,3);
	
	return true;
}
bool TestApp::Start()
{
	float points[] = {
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
	glLinkProgram(shader_programme);
	return true;
}

bool TestApp::Shutdown()
{
	return true;
}




bool TestApp::Run(unsigned int width,unsigned int height, const char* appname, bool fullscreen)
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
	float deltaTime = 0;
	float currentTime = glfwGetTime();
	float prevTime = 0;
	glfwSwapInterval(0);
	Start();
	while (this->m_running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (glfwWindowShouldClose(this->window))
			this->m_running = false;
		if (glfwGetKey(this->window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(this->window, true);
		glfwPollEvents();
		this->Update(deltaTime);
		prevTime = currentTime;
		currentTime = float(glfwGetTime());
		deltaTime = (currentTime - prevTime);
		std::cout << (int)(1.f / deltaTime) << std::endl;

		this->Draw();
		glfwSwapBuffers(this->window);
	}
	glfwDestroyWindow(this->window);
	glfwTerminate();
	return true;

}
