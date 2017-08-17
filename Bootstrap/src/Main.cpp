#include<gl_core_4_4.h>
#include<glfw\glfw3.h>

#include <glm/glm.hpp>
#include<assert.h>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
void testupdate();
int main()
{
	if (!glfwInit())
		return -1;
	GLFWwindow * window = glfwCreateWindow(1200, 800, "TEST", nullptr, nullptr);

	assert(window != nullptr);
	glfwMakeContextCurrent(window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n ", major, minor);

	glm::vec4 chickenNuggets = glm::vec4(.8f, .3f, .75f, 1);
	glClearColor(chickenNuggets.r, chickenNuggets.g, chickenNuggets.b, chickenNuggets.a);
	bool b = false;
	bool * bp = &b;

	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui_ImplGlfwGL3_NewFrame();
	
	ImGui::Begin("TEST");
	while (true)
	{
		ImGui_ImplGlfwGL3_NewFrame();
		
		glClear((GL_COLOR_BUFFER_BIT) | GL_DEPTH_BUFFER_BIT);

		if (glfwWindowShouldClose(window))
			break;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_C))
		{
			chickenNuggets.r -= .002f;
			chickenNuggets.g -= .002f;
			chickenNuggets.b -= .002f;
			glClearColor(chickenNuggets.r, chickenNuggets.g, chickenNuggets.b, chickenNuggets.a);

		}

		glfwPollEvents();
		glfwSwapBuffers(window);
		ImGui::Render();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
void testupdate()
{

}