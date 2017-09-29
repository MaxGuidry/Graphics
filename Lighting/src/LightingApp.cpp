#include "LightingApp.h"
#include "gl_core_4_4.h"
#include <glm/ext.hpp>
#include <glfw/include/glfw/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <MaxGizmos.h>
#include <tiny_obj_loader.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "ReadFile.h"
#include "imgui_internal.h"


Shader frag;
Shader vert;
unsigned int vao;
unsigned int vbo;
unsigned int ibo;
unsigned int indexcount;
LightingApp::LightingApp() : camera(new DollyCamera()), m_fill(false), m_PhongShader(0), objects(std::vector<Mesh>())
{
}


LightingApp::~LightingApp()
{
}

bool LightingApp::Start()
{
	ImGui::CreateContext();
	ImGui::GetIO().DisplaySize = ImVec2(20, 20);
	ImGui_ImplGlfwGL3_Init(window, true);

	camera->LookAt(camera->m_position, glm::vec3(0), glm::vec3(0, 1, 0));

	//Mesh tmp = MaxGizmos::GenSphere(1.f, 32, 32);
	//m_sphere.initialize(tmp.getVerts(), tmp.getIndices());
	//m_sphere.create_buffers();

	m_directLight.diffuse = glm::vec3(0, 1, 0);
	m_directLight.specular = glm::vec3(1);
	m_ambientLight = glm::vec3(0, .25f, 0);

	m_material.diffuse = glm::vec3(1);
	m_material.ambient = glm::vec3(1);
	m_material.specular = glm::vec3(1);

	m_material.specularPower = 30;

	generateSphere(70, 70, vao, vbo, ibo, indexcount);

	m_PhongShader = glCreateProgram();
	vert = Shader(m_PhongShader);
	vert.load("./phongv.vert", GL_VERTEX_SHADER);
	vert.attach();
	frag = Shader(m_PhongShader);
	frag.load("./phongf.frag", GL_FRAGMENT_SHADER);
	frag.attach();
	return true;
}
static double mousex = 0;
static double mousey = 0;
static double pmouseX = 0;
static double pmouseY = 0;
static bool F1 = false;
static bool F5 = false;
static bool blinn = false;;


bool LightingApp::Update(float deltaTime)
{
	/*if (ImGui::BeginMenu("Phong Frag"))
	{
		auto sd = ShaderData{ shader, fbuffer, GL_FRAGMENT_SHADER, false };
		ImGui::InputTextMultiline("Shader", fbuffer, sizeof fbuffer, ImGui::GetWindowSize(),
			ImGuiInputTextFlags_CallbackAlways, TextEditCallBackStub, static_cast<void*>(&sd));
		ImGui::EndMenu();
	}*/

	static bool mouse_down = false;
	if (!ImGui::IsAnyItemActive())
	{
		if (mouse_down == false)
		{
			mouse_down = true;
			glfwGetCursorPos(window, &mousex, &mousey);
		}

		glfwGetCursorPos(window, &mousex, &mousey);
		glm::vec2 cpos = glm::vec2(mousex, mousey);

		glm::vec2 deltaMouse = glm::vec2(mousex - pmouseX, mousey - pmouseY);
		deltaMouse = deltaMouse * deltaTime;

		if (glfwGetMouseButton(window, 0))
			camera->LookAround(deltaMouse);

		if (glfwGetKey(window, GLFW_KEY_D))
		{
			glm::vec3 right = glm::vec3(camera->m_view[0][0], camera->m_view[1][0], camera->m_view[2][0]);
			camera->setPosition(right * .25f);
		}
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			glm::vec3 forward = glm::vec3(camera->m_view[0][2], camera->m_view[1][2], camera->m_view[2][2]);
			camera->setPosition(-forward* .25f);
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			glm::vec3 forward = glm::vec3(camera->m_view[0][2], camera->m_view[1][2], camera->m_view[2][2]);
			camera->setPosition(forward* .25f);
		}
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			glm::vec3 right = glm::vec3(camera->m_view[0][0], camera->m_view[1][0], camera->m_view[2][0]);
			camera->setPosition(-right* .25f);
		}

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
	float time = glfwGetTime();
	m_directLight.direction = glm::normalize(glm::vec3(-1, -1, -1));
	if (glfwGetKey(window, GLFW_KEY_UP))
		m_material.specularPower += .2f;
	if (glfwGetKey(window, GLFW_KEY_DOWN))
		m_material.specularPower -= .2f;
	//std::cout << m_material.specularPower << std::endl;
	m_directLight.direction = glm::normalize(glm::vec3(10, 10, 10));
	//m_directLight.diffuse = glm::vec3(sin(time), sin(2.f*time), cos(time));
	//m_directLight.direction = glm::normalize(glm::vec3(sin(time/2.f), 0, cos(time/2.f)));

	bool F5_held = F5;

	if (glfwGetKey(window, GLFW_KEY_F5))
		F5 = true;
	else
		F5 = false;

	if (F5 && F5_held != F5)
		blinn = !blinn;
	if (!blinn)
		frag.reload("./phongf.frag", GL_FRAGMENT_SHADER);
	else
		frag.reload("./blinnphong.frag", GL_FRAGMENT_SHADER);
	frag.attach();

	return true;
}


bool LightingApp::Shutdown()
{
	return true;
}

bool LightingApp::Draw()
{

	ImGui_ImplGlfwGL3_NewFrame();
	glUseProgram(m_PhongShader);
	//m_sphere.bind();
	unsigned int pvU = vert.getUniform("ProjectionViewModel");
	glUniformMatrix4fv(pvU, 1, false, glm::value_ptr(camera->getProjectionView() * glm::scale(glm::vec3(5))));
	int lightUniform = frag.getUniform("direction");
	glUniform3fv(lightUniform, 1, glm::value_ptr(-m_directLight.direction));

	lightUniform = frag.getUniform("Id");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_directLight.diffuse));

	lightUniform = frag.getUniform("Ia");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_ambientLight));

	lightUniform = frag.getUniform("Ka");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_material.ambient));

	lightUniform = frag.getUniform("Ks");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_material.specular));

	lightUniform = frag.getUniform("Kd");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_material.diffuse));

	lightUniform = frag.getUniform("Is");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_directLight.specular));

	lightUniform = frag.getUniform("a");
	glUniform1f(lightUniform, m_material.specularPower);

	lightUniform = frag.getUniform("camforward");
	//glUniform3fv(lightUniform, 1, glm::value_ptr(glm::vec3(camera->getView()[0][2], camera->getView()[1][2], camera->getView()[2][2])));
	glUniform3fv(lightUniform, 1, glm::value_ptr(camera->m_position));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
	//m_sphere.draw(GL_TRIANGLE_STRIP);
	//m_sphere.unbind();


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ImGui::ShowTestWindow();
	ImGui::Begin("test");
	ImGui::SetWindowSize(ImVec2(700, 700));

	ImGui::BeginGroup();
	float ambient[3] = { m_ambientLight.r,m_ambientLight.g,m_ambientLight.b };
	ImGui::ColorEdit3("Ambient Light", ambient);
	m_ambientLight = glm::vec3(ambient[0], ambient[1], ambient[2]);
	float diffuse[3] = { m_directLight.diffuse.r,m_directLight.diffuse.g,m_directLight.diffuse.b };
	ImGui::ColorEdit3("Diffuse Light", diffuse);
	m_directLight.diffuse = glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
	float spec[3] = { m_directLight.specular.r,m_directLight.specular.g,m_directLight.specular.b };
	ImGui::ColorEdit3("Specular Light", spec);
	m_directLight.specular = glm::vec3(spec[0], spec[1], spec[2]);
	ImGui::EndGroup();
	ImGui::End();
#pragma region Object Menu
	//ImGui::ItemSize(ImVec2(3, 20));
	ImGui::Begin("Object Menu");
	ImGui::SetWindowSize(ImVec2(100, 100));
	if (ImGui::Button("Add Sphere", ImVec2(70, 15)))
	{

		Mesh m = MaxGizmos::GenSphere(1.f, 25, 25);
		m.create_buffers();
		m.m_position = glm::vec3(objects.size(), objects.size(), objects.size());
		objects.push_back(m);
	}
	ImGui::End();

#pragma endregion 
	ImGui::Begin("test move");
	ImGui::SetWindowSize(ImVec2(500, 500));
	int i = 0;
	for (auto m : objects)
	{
		std::string s = std::to_string(i);
		s = "sphere " + s;
		char const *pchar = s.c_str();
		if (ImGui::Button(pchar, ImVec2(70, 20)))
			m_selectedObject = &objects[i];
		i++;
	}
	if (m_selectedObject != nullptr)
	{
		if (ImGui::DragFloat("X", &(m_selectedObject->m_position.x), .05f, -9000, 9000))
			if (ImGui::GetMousePos().x > 890)
			{
				
				
				
				
			}
		ImGui::DragFloat("Y", &(m_selectedObject->m_position.y), .05f, -9000, 9000);
		ImGui::DragFloat("Z", &(m_selectedObject->m_position.z), .05f, -9000, 9000);
	}
	ImGui::End();
	ImGui::Render();

	if (!m_fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (auto m : objects)
	{

		unsigned int pvm = vert.getUniform("ProjectionViewModel");
		glUniformMatrix4fv(pvm, 1, false, glm::value_ptr(camera->getProjectionView() *glm::translate(m.m_position)));


		m.bind();
		m.draw();
		m.unbind();
	}


	glUseProgram(0);

	return true;
}
void LightingApp::generateSphere(unsigned int segments, unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount)
{

	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	// using AIEVertex for now, but could be any struct as long as it has the correct elements
	Vertex* vertices = new Vertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	float ringAngle = glm::pi<float>() / (rings + 1);
	float segmentAngle = 2.0f * glm::pi<float>() / segments;

	Vertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring)
	{
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment, ++vertex)
		{
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = glm::vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);
			vertex->normal = glm::vec4(x0, y0, z0, 0);

			vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);

			// not a part of the AIEVertex, but this is how w generate bitangents
			vertex->bitangent = glm::vec4(glm::cross(glm::vec3(vertex->normal), glm::vec3(vertex->tangent)), 0);

			vertex->texcoord = glm::vec2(segment / (float)segments, ring / (float)(rings + 1));
		}
	}

	unsigned int index = 0;
	for (unsigned i = 0; i < (rings + 1); ++i)
	{
		for (unsigned j = 0; j < segments; ++j)
		{
			indices[index++] = i * (segments + 1) + j;
			indices[index++] = (i + 1) * (segments + 1) + j;
			indices[index++] = i * (segments + 1) + (j + 1);

			indices[index++] = (i + 1) * (segments + 1) + (j + 1);
			indices[index++] = i * (segments + 1) + (j + 1);
			indices[index++] = (i + 1) * (segments + 1) + j;
		}
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &vao);

	// all changes will apply to this handle
	glBindVertexArray(vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	// texcoords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	// tangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3 + sizeof(glm::vec2)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
	delete[] vertices;
}