#include "RenderingApp.h"
#include"ReadFile.h"
#include<glm/ext.hpp>
#include <gl_core_4_4.h>
#include <glfw/include/glfw/glfw3.h>
#include <vector>
#include "tiny_obj_loader.h"

Mesh circle = Mesh();
Mesh sphere = Mesh();
glm::mat4 grid =
glm::mat4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	-5, 0, -5, 1);
glm::mat4 circleT =
glm::mat4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, -1, 0, 1);
RenderingApp::RenderingApp() : camera(new DollyCamera())
{
}


RenderingApp::~RenderingApp()
{
}

Mesh * RenderingApp::GenGrid(unsigned int rows, unsigned int cols)
{
	Vertex * aoVerts = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVerts[r * cols + c].position = glm::vec4(float(c), 0, float(r), 1);
			//glm::vec3 color = glm::vec3(sinf( 12 * cosf(10 * tanf(float(c) / (float(cols))))), cosf(9 * sinf(21 * tanf(float(c) / (float(cols))))), tanf(34 * cosf(24 * sinf(float(c) / (float(cols))))));
			glm::vec3 color = glm::vec3((float)r / (float)rows, ((float)r / (float)rows) * ((float)c / (float)cols), (float)c / (float)cols);
			aoVerts[r * cols + c].color = glm::vec4(color, 1);
		}
	unsigned int * auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	unsigned int index = 0;
	std::vector<Vertex> verts = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	for (unsigned int r = 0; r < (rows - 1); ++r)
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}





	for (unsigned int i = 0; i < (rows * cols); i++)
		verts.push_back(aoVerts[i]);
	for (unsigned int i = 0; i < index; i++)
		indices.push_back(auiIndices[i]);



	Mesh * gridMesh = new Mesh();
	gridMesh->initialize(verts, indices);

#pragma region circle
	//Vertex * aoVerts = new Vertex[100];
	//unsigned int * auiIndices = new unsigned int[200];

	//unsigned int i = 0;
	//unsigned int ai =0;
	//for (float x = -2.f; x < 2.f; x += .08f)
	//{
	//	aoVerts[i].position = glm::vec4(x, 0, glm::sqrt(4.f - x*x), 1);
	//	//std::cout << x << std::endl;
	//	//std::cout << aoVerts[i].position.x << "," << aoVerts[i].position.y << "," << aoVerts[i].position.z << std::endl;
	//	auiIndices[ai] = i;
	//	ai++;
	//	auiIndices[ai] = i;
	//	ai++;
	//	i++;
	//}

	//for (float x = 2.f; x > -2.f; x -= .08f)
	//{
	//	aoVerts[i].position = glm::vec4(x, 0, -glm::sqrt(4.f - x*x), 1);
	//	//std::cout << x << std::endl;
	//	//std::cout << aoVerts[i].position.x << "," << aoVerts[i].position.y << "," << aoVerts[i].position.z << std::endl;
	//	auiIndices[ai] = i;
	//	ai++;
	//	auiIndices[ai] = i;
	//	ai++;
	//	i++;
	//}
#pragma endregion 

		gridMesh->create_buffers();
	gridMesh->bind();
	verts.clear();
	indices.clear();
	delete[] auiIndices;
	delete[] aoVerts;
	return gridMesh;
}

Mesh box = Mesh();

bool RenderingApp::Start()
{

	camera->LookAt(glm::vec3(15, 15, 15), glm::vec3(5, 0, 5), glm::vec3(0, 1, 0));
	const char * vsSource = FileRead("vertex.vert");
	const char * fsSource = FileRead("fragment.frag");
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	delete vsSource;
	delete fsSource;
	mesh = GenGrid(3, 3);
	Vertex a = { glm::vec4(0,0,0,1),glm::vec4(1,1,1,1) };
	Vertex b = { glm::vec4(1,0,0,1),glm::vec4(1,1,1,1) };
	Vertex c = { glm::vec4(0,1,0,1),glm::vec4(1,1,1,1) };
	Vertex d = { glm::vec4(1,1,0,1),glm::vec4(1,1,1,1) };
	Vertex e = { glm::vec4(0,1,1,1),glm::vec4(1,1,1,1) };
	Vertex f = { glm::vec4(1,1,1,1),glm::vec4(1,1,1,1) };
	Vertex g = { glm::vec4(0,0,1,1),glm::vec4(1,1,1,1) };
	Vertex h = { glm::vec4(1,0,1,1),glm::vec4(1,1,1,1) };


	std::vector<Vertex> boxVerts{ a,b,c,d,e,f,g,h };
	std::vector<unsigned int> boxindeces
	{
		0,2,1,
		1,2,3,

		2,4,3,
		3,4,5,

		4,6,5,
		5,6,7,

		1,3,7,
		7,3,5,

		6,0,7,
		7,0,1,

		6,4,0,
		0,4,2

	};

#pragma region Circle
	std::vector<Vertex> circleVerts = std::vector<Vertex>();
	std::vector<unsigned int> circleIndices = std::vector<unsigned>();
	unsigned int i = 0;
	for (float x = -2.f; x < 2.f; x += .08f)
	{
		circleVerts.push_back(Vertex{ glm::vec4(x, 0, glm::sqrt(4.f - x*x), 1),glm::vec4(1,1,1,1) });
		circleIndices.push_back(i);
		i++;

		circleIndices.push_back(i);
	}
	circleIndices.push_back(i);
	for (float x = 2.f; x > -2.f; x -= .08f)
	{
		circleVerts.push_back(Vertex{ glm::vec4(x, 0, -glm::sqrt(4.f - x*x), 1),glm::vec4(1,1,1,1) });
		if (i + 1 < 100)
			circleIndices.push_back(i + 1);
		i++;

		if (i < 100)
			circleIndices.push_back(i);

	}

	circleIndices.push_back(0);
	/*circleVerts.push_back(Vertex{ glm::vec4(0,2,0,0),glm::vec4(1,1,1,1) });
	circleVerts.push_back(Vertex{ glm::vec4(1.4,1.4,0,0),glm::vec4(1,1,1,1) });
	circleVerts.push_back(Vertex{ glm::vec4(2,0,0,0),glm::vec4(1,1,1,1) });
	circleVerts.push_back(Vertex{ glm::vec4(1.4,-1.4,0,0),glm::vec4(1,1,1,1) });
	circleVerts.push_back(Vertex{ glm::vec4(0,-2,0,0),glm::vec4(1,1,1,1) });
	circleVerts.push_back(Vertex{ glm::vec4(-1.4,-1.4,0,0),glm::vec4(1,1,1,1) });
	circleVerts.push_back(Vertex{ glm::vec4(-2,0,0,0),glm::vec4(1,1,1,1) });
	circleVerts.push_back(Vertex{ glm::vec4(-1.4,1.4,0,0),glm::vec4(1,1,1,1) });

	for (unsigned int i = 0; i < 8; i++)
	{
		circleIndices.push_back(i);
		circleIndices.push_back(i);
	}*/
	circle.initialize(circleVerts, circleIndices);
	circle.create_buffers();

#pragma endregion 


#pragma region SPHERE
	std::vector<Vertex> sphereVerts = std::vector<Vertex>();
	std::vector<unsigned int> sphereIndices = std::vector < unsigned int>();
	float y = 1.f;
	float x = 0.f;
	float z = 0.f;


#pragma endregion
	box.initialize(boxVerts, boxindeces);
	box.create_buffers();
	box.bind();
	return true;
}
static double mousex = 0;
static double mousey = 0;
static double pmouseX = 0;
static double pmouseY = 0;


bool RenderingApp::Update(float deltaTime)
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
	deltaMouse = deltaMouse * deltaTime * 15.f;

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

	return true;
}

bool RenderingApp::Shutdown()
{
	return true;
}

bool RenderingApp::Draw()
{
	mesh->bind();
	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	unsigned int time = glGetUniformLocation(m_programID, "time");
	glUniform1f(time, glfwGetTime());

	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->getProjectionView()));
	glDrawElements(GL_TRIANGLES, mesh->index_Count, GL_UNSIGNED_INT, 0);
	mesh->unbind();
	box.bind();
	grid = grid * glm::mat4(
		glm::cos(.01f), 0, -glm::sin(.01f), 0,
		0, 1, 0, 0,
		glm::sin(.01f), 0, glm::cos(.01f), 0,
		0, 0, 0, 1);
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->getProjectionView()*  grid));
	glDrawElements(GL_TRIANGLES, box.index_Count, GL_UNSIGNED_INT, 0);
	box.unbind();
	circle.bind();
	glm::mat4 x = glm::mat4(
		glm::cos(.05f), glm::sin(.05f), 0, 0,
		-glm::sin(.05f), glm::cos(.05f), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
	glm::mat4 y = glm::mat4(
		1, 0, 0, 0,
		0, glm::cos(.05f), glm::sin(.05f), 0,
		0, -glm::sin(.05f), glm::cos(.05f), 0,
		0, 0, 0, 1
		);
	circleT = circleT * x * y;
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->getProjectionView() * circleT));
	glDrawElements(GL_LINES, circle.index_Count, GL_UNSIGNED_INT, 0);
	circle.unbind();
	glUseProgram(0);
	return true;
}
