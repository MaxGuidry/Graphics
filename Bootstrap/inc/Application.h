#pragma once
#include <gl_core_4_4.h>
//#include <glfw/glfw3.h>
#include<assert.h>
#include<iostream>
struct GLFWwindow;
class Application
{
public:
	 Application();
	virtual ~Application();
	//virtual bool Run(unsigned int width, unsigned int height, const char * appname, bool fullscreen);
	GLFWwindow * window;
	bool m_running;
protected:
	virtual bool Start() = 0;
	virtual bool Update(float deltaTime) = 0;
	virtual bool Shutdown() = 0;
	virtual bool Draw() = 0;

	
};





