#pragma once
#include "Application.h"
class TestApp :
	public Application
{
public:
	TestApp();
	~TestApp();
	bool Update(float deltaTime) override;
	bool Draw()override;
	bool Start()override;
	bool Shutdown()override;
	/*
	 runs the application
	@param width the width of the window
	@param height the height of the window
	@param appname the name of the window
	@param fullscreen or not
	 */
	bool Run(unsigned int width, unsigned int height, const char * appname, bool fullscreen);
	GLuint vao = 0;
	GLuint shader_programme;
};

