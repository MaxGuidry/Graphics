#include "Application.h"
#include <glfw/glfw3.h>

Application::Application(): window(nullptr), m_running(true)
{
}

 Application::~Application()
 {
	 
 }

 

 bool Application::Run(unsigned int width, unsigned int height, const char* appname, bool fullscreen)
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
	 //glfwSwapInterval(0);
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
		 this->Update(static_cast<double>(deltaTime));
		 prevTime = currentTime;
		 currentTime = float(glfwGetTime());
		 deltaTime = (currentTime - prevTime);
		 //std::cout << (int)(1.f / deltaTime) << std::endl;

		 this->Draw();
		 glfwSwapBuffers(this->window);
	 }
	 glfwDestroyWindow(this->window);
	 glfwTerminate();
	 return true;

 }