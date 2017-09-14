#include "RenderingApp.h"
#include<glm/ext.hpp>
#include <gl_core_4_4.h>
#include <glfw/include/glfw/glfw3.h>
#include <vector>
#include "tiny_obj_loader.h"
#include "Shader.h"
#include "MaxGizmos.h"


Mesh circle = Mesh();
Mesh SPHERE;
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
RenderingApp::RenderingApp() : camera(new DollyCamera()), m_fill(false)
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
	gridMesh->create_buffers();
	gridMesh->bind();
	verts.clear();
	indices.clear();
	delete[] auiIndices;
	delete[] aoVerts;
	return gridMesh;
}

Mesh* RenderingApp::GenSphereWithCircleEquation(float radius, unsigned verts)
{
	std::vector<Vertex> sphereVerts = std::vector<Vertex>();
	std::vector<unsigned int> sphereIndices = std::vector < unsigned int>();

	//Vertex * verts = new Vertex[];
	for (float y = radius; y > -radius; y -= (radius / (float)verts) * 2.f)
	{
		float x = glm::sqrt((radius * radius) - (y * y));
		for (float n = x; n > -x; n -= (radius / (float)verts) * 2.f)
		{
			float z = glm::sqrt((x * x) - (n * n));

			Vertex v = Vertex();
			v.position = glm::vec4(n, y, z, 1);
			v.color = glm::vec4(1, 1, 1, 1);
			if (z != 0.f)
			{
				Vertex vneg = Vertex();
				vneg.position = glm::vec4(n, y, -z, 1);
				vneg.color = glm::vec4(1, 1, 1, 1);
				sphereVerts.push_back(v);
				sphereVerts.push_back(vneg);
			}


		}
		Vertex v = Vertex();
		v.position = glm::vec4(x, y, 0, 1);
		v.color = glm::vec4(x, y, 0, 1);
		Vertex vneg = Vertex();
		sphereVerts.push_back(v);
	}
	Mesh * sp_here = new Mesh();
	sp_here->initialize(sphereVerts, std::vector<unsigned int>());
	return sp_here;
}


Mesh box = Mesh();

bool RenderingApp::Start()
{

	camera->LookAt(glm::vec3(15, 15, 15), glm::vec3(5, 0, 5), glm::vec3(0, 1, 0));

	m_StandardShaderID = glCreateProgram();


	Shader * vertexShader = new Shader(m_StandardShaderID);
	vertexShader->load("./vertex.vert", GL_VERTEX_SHADER);
	vertexShader->attach();
	Shader * fragShader = new Shader(m_StandardShaderID);
	fragShader->load("./fragment.frag", GL_FRAGMENT_SHADER);
	fragShader->attach();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
	float radius = 1.f;
	unsigned int circleSize = 10;
	unsigned int meridians = 10;
	std::vector<Vertex> sphereverts = std::vector<Vertex>();
	
	Mesh m = MaxGizmos::GenSphere(1.f, 10, 10);
	SPHERE.initialize(m.getVerts(), m.getIndices());
	SPHERE.create_buffers();
	box.initialize(boxVerts, boxindeces);
	box.create_buffers();
	box.bind();
	return true;
}
static double mousex = 0;
static double mousey = 0;
static double pmouseX = 0;
static double pmouseY = 0;
static bool F1 = false;

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
	deltaMouse = deltaMouse * deltaTime * 3.f;

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
	return true;
}

bool RenderingApp::Shutdown()
{
	return true;
}

bool RenderingApp::Draw()
{
	mesh->bind();
	glUseProgram(m_StandardShaderID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_StandardShaderID, "projectionViewWorldMatrix");
	unsigned int time = glGetUniformLocation(m_StandardShaderID, "time");
	glUniform1f(time, glfwGetTime());
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->getProjectionView() * glm::translate(glm::vec3(1,0,-1))));
	mesh->draw(GL_TRIANGLES);
	mesh->unbind();
	box.bind();
	grid = grid * glm::mat4(
		glm::cos(.01f), 0, -glm::sin(.01f), 0,
		0, 1, 0, 0,
		glm::sin(.01f), 0, glm::cos(.01f), 0,
		0, 0, 0, 1);
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->getProjectionView()*  grid));
	box.draw(GL_TRIANGLES);
	box.unbind();
	circle.bind();
	glm::mat4 x = glm::mat4(
		glm::cos(.05f), glm::sin(.05f), 0, 0,
		-glm::sin(.05f), glm::cos(.05f), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	glm::mat4 z = glm::mat4(
		1, 0, 0, 0,
		0, glm::cos(.05f), glm::sin(.05f), 0,
		0, -glm::sin(.05f), glm::cos(.05f), 0,
		0, 0, 0, 1
	);
	circleT = circleT * x * z;
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->getProjectionView() * circleT));
	glDrawElements(GL_LINES, circle.index_Count, GL_UNSIGNED_INT, 0);
	circle.unbind();
	SPHERE.bind();
	glm::mat4 scale5 = glm::scale(glm::vec3(5, 5, 5));
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->getProjectionView() * glm::translate(glm::vec3(0, -10, 0))  * scale5));
	//glDrawArrays(GL_POINTS, 0, sphere->vertRef.size());
	SPHERE.draw(GL_TRIANGLE_STRIP);

	SPHERE.unbind();
	glUseProgram(0);
	return true;
}

