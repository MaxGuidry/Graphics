#include "RenderingApp.h"
#include"ReadFile.h"
#include<glm/ext.hpp>
#include <gl_core_4_4.h>
#include <glfw/include/glfw/glfw3.h>
#include "gl_core_4_4.h"
#include "gl_core_4_4.h"

RenderingApp::RenderingApp() : camera(new DollyCamera())
{
}


RenderingApp::~RenderingApp()
{
}

void RenderingApp::GenGrid(unsigned rows, unsigned cols)
{
	Vertex * aoVerts = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVerts[r * cols + c].position = glm::vec4(float(c), 0, float(r), 1);
			//glm::vec3 color = glm::vec3(sinf( 12 * cosf(10 * tanf(float(c) / (float(cols))))), cosf(9 * sinf(21 * tanf(float(c) / (float(cols))))), tanf(34 * cosf(24 * sinf(float(c) / (float(cols))))));
			glm::vec3 color = glm::vec3(sinf(r * glfwGetTime()), sinf(c), sinf(r*c));
			aoVerts[r * cols + c].color = glm::vec4(color, 1);
		}
	unsigned int * auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	unsigned int index = 0;
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
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
	indexCount = (rows - 1) * (cols - 1) * 6;
	//indexCount = 200;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] auiIndices;
	delete[] aoVerts;

}

bool RenderingApp::Start()
{

	camera->LookAt(glm::vec3(15, 15, 15), glm::vec3(5, 0, 5), glm::vec3(0, 1, 0));
	const char * vsSource = FileRead("./vertex.vert");
	const char * fsSource = FileRead("./fragment.frag");
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
	if (success == GL_FALSE) {
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
	GenGrid(10, 10);

	return true;
}

bool RenderingApp::Update(float deltaTime)
{
	return true;
}

bool RenderingApp::Shutdown()
{
	return true;
}

bool RenderingApp::Draw()
{
	glBindVertexArray(vao);
	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	unsigned int time = glGetUniformLocation(m_programID, "time");
	glUniform1f(time, glfwGetTime());
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->getProjectionView()));
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	return true;
}
