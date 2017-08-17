#include<gl_core_4_4.h>
#include<glfw\glfw3.h>
#include<Gizmos.h>
#include <glm/glm.hpp>
#include<assert.h>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include<glm\ext.hpp>
int main()
{
	if (!glfwInit())
		return -1;
	GLFWwindow * window = glfwCreateWindow(1600, 900, "TEST", nullptr, nullptr);

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
	bool button = false;
	ImVec4 clear_color = ImColor(114, 144, 154);
	ImGui::Begin("TEST");
	glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::pi<float>() * .25f, 16.0f / 9.0f,.1f, 1000.f);
	glm::mat4 spheremat = 
		glm::mat4(
			1, 0, 0, 0,
			0, 1, 0, 0, 
			0, 0, 1, 0, 
			0, 0, 0, 1);
	float theta = 0.002f;
	Gizmos::create();
	glEnable(GL_DEPTH_TEST);
	float  f = 0.0f;
	while (true)
	{
		ImGui_ImplGlfwGL3_NewFrame();
		if (ImGui::Button("THIS IS A TEST"))
			button = !button;
		if (button)
			ImGui::Text("We hit that button!");
		ImGui::SliderFloat("Color", &f, 0.0f, 255.0f);
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
		glm::mat4 rotx = 
			glm::mat4(
				1, 0, 0, 0,
				0, glm::cos(theta), -glm::sin(theta), 0,
				0, glm::sin(theta), glm::cos(theta), 0, 
				0, 0, 0, 1);
		glm::mat4 roty =
			glm::mat4(
				glm::cos(theta), 0, -glm::sin(theta), 0,
				0, 1, 0, 0,
				glm::sin(theta), 0, glm::cos(theta), 0,
				0, 0, 0, 1);
		glm::mat4 rotz =
			glm::mat4(
				glm::cos(theta), -glm::sin(theta), 0, 0,
				glm::sin(theta), glm::cos(theta), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
		spheremat= spheremat * (rotx  * rotz) ;
		glm::mat4 * matptr = &spheremat;
		Gizmos::addSphere(glm::vec3(0,0,0), 5.f, 50, 50,black,matptr);
		Gizmos::draw(proj * view);
		glfwPollEvents();
		ImGui::Render();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
