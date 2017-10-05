#include "TextureApp.h"
#include "gl_core_4_4.h"
#include<glfw/include/GLFW/glfw3.h>
#include "Shader.h"

#include<glm/ext.hpp>
#include<glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "MaxGizmos.h"

float maxNoise(glm::vec2 verts, unsigned int dims, int seed)
{
	
	
	int prime = 36343;
	if (seed == 0)
		seed = prime;
	if (dims == 0)
		dims = prime;
	float c = (prime / seed) << dims / prime;
	while (c == 0)
	{
		c = (prime << seed);
	}

	float num = prime / float(seed) * c;

	float numa = sqrt(glm::abs(c));
	if (numa == 0)
		numa++;
	float numb = glm::fract(float(prime) / sqrt(glm::abs(c + prime)));
	if (numb == 0)
		numb++;
	float noise = 0.f;
	float x = verts.x / float(dims);
	float y = verts.y / float(dims);
	x = glm::ceil(x * num + float(prime) / numa);
	y = glm::ceil(y * prime * float(num) / numb);
	float x2 = (verts.x - prime) / float(dims);
	float y2 = (verts.y - dims) / float(dims);
	x2 = glm::ceil(x2 * num + float(prime) / numa);
	y2 = glm::ceil(y2 * prime * float(num) / numb);
	float x3 = (verts.x + dims) / float(dims);
	float y3 = (verts.y + prime) / float(dims);
	x3 = glm::ceil(x3 * num + float(prime) / numa);
	y3 = glm::ceil(y3 * prime * float(num) / numb);
	glm::vec4 lerped = glm::vec4(x, y, 1, 1);// glm::vec4(glm::lerp(glm::vec2(x, y), glm::vec2(x2, y), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x, y2), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x2, y2), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x, y3), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x3, y), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x3, y3), .1f), 1, 1);
	/*if (seed > prime)
		lerped = lerped * glm::rotate(float(sqrt(abs(c))), glm::vec3(numa, numb, glm::fract(prime / c)));*/
		//if (seed > dims)
		//	lerped = lerped * glm::rotate(float(sqrt(abs(prime))), glm::vec3(c, prime, glm::fract(numb)));
	if (numa < numb)
		lerped = lerped * glm::rotate(float(sqrt(abs(c / prime))), glm::vec3(numa, c, glm::fract(float(t))));
	else
		lerped = lerped * glm::rotate(float(sqrt(abs(c))), glm::vec3(seed, t, glm::fract(c)));
	noise = (glm::normalize(lerped).x + glm::normalize(lerped).y + glm::normalize(lerped).z + glm::normalize(lerped).w);
	noise = glm::fract(noise);
	if (dims == seed && noise <1 && noise>-1)
		return noise;
	if (noise <1.f && noise>-1.f)
		return noise * maxNoise(verts, numa, numa);
	else
		return glm::fract(noise);//noise = maxNoise(glm::vec2(noise, c), numb, numa);

	return noise;


}

void generateSphere(unsigned int segments, unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount);
unsigned int vao;
unsigned int vbo;
unsigned int ibo;
unsigned int indexcount;
Shader vert;
Shader frag;
GLuint shader;
TextureApp::TextureApp() : GridMesh(), cam()
{
}


TextureApp::~TextureApp()
{
}

bool TextureApp::Start()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImGui::CreateContext();
	ImGui::GetIO().DisplaySize = ImVec2(20, 20);
	ImGui_ImplGlfwGL3_Init(window, true);
	m_directLight.diffuse = glm::vec3(1);
	m_directLight.specular = glm::vec3(1);
	m_ambientLight = glm::vec3(1);

	m_material.diffuse = glm::vec3(1);
	m_material.ambient = glm::vec3(1);
	m_material.specular = glm::vec3(1);

	m_material.specularPower = 30;
	m_directLight.direction = glm::vec3(1, 1, 1);
	cam.LookAt(cam.m_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	Mesh grid = GenGrid(64, 64);
	shader = glCreateProgram();
	vert = Shader(shader);
	vert.load("vertex.vert", GL_VERTEX_SHADER);
	vert.attach();
	frag = Shader(shader);
	frag.load("fragment.frag", GL_FRAGMENT_SHADER);
	frag.attach();
	generateSphere(70, 70, vao, vbo, ibo, indexcount);
	//GridMesh->loadTexture("./space.png", STBI_default);
	Mesh m = MaxGizmos::GenSphere(1, 50, 50);
	m.create_buffers();
	//m.m_position = glm::vec3(objects.size(), objects.size(), objects.size());
	//m.loadTexture("./space.png", STBI_rgb_alpha);
	m.create_buffers();
	objects.push_back(m);
	//objects.push_back(*GridMesh);
	//m.loadTexture("./crate.png", STBI_rgb_alpha);
	//grid.loadTexture("./crate.png", STBI_rgb_alpha);

	int dims = 64;
	int seed = 7.f;
	float *perlinData = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 6;
	for (int x = 0; x < dims; ++x)
	{
		for (int y = 0; y < dims; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlinData[y * dims + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				//float perlinSample = glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;

				float perlinSample = maxNoise(glm::vec2(float(x), float(y) * scale*freq), dims, seed) *.5f + .5f;
				//float perlin2 = maxNoise(glm::vec2(float(x), float(y)) * scale * o * freq, dims, seed / scale);

				//std::cout << perlinSample << std::endl;
				perlinData[y * dims + x] += perlinSample * amplitude;

				amplitude *= persistence;
				/*if (o != 0)
					seed += int(pow(o , x+y)) << o;*/
			}
			perlinData[y * dims + x] = glm::fract(perlinData[y * dims + x]);

			int f = (y * dims + x);
			//if (f > dims && f < (dims * dims) - 1)
				//perlinData[f] = (perlinData[f+1] + perlinData[f] + perlinData[f-dims]) / 4.f;
			std::cout << perlinData[f] << std::endl;
		}
	}


	m.loadNoise(dims, dims, perlinData);
	grid.loadNoise(dims, dims, perlinData);
	grid.create_buffers();
	objects.push_back(grid);

	return true;
}
static double mousex = 0;
static double mousey = 0;
static double pmouseX = 0;
static double pmouseY = 0;
static bool F1 = false;
static bool F5 = false;
float timeHeld = 1;
bool TextureApp::Update(float deltaTime)
{




	if (glfwGetKey(window, GLFW_KEY_F12))
		selectedMesh->loadTexture(selectedMesh->texture.textureFile, STBI_default);
	glfwGetCursorPos(window, &mousex, &mousey);
	glm::vec2 cpos = glm::vec2(mousex, mousey);
	glm::vec3 oldCam = cam.m_position;

	glm::vec2 deltaMouse = glm::vec2(mousex - pmouseX, mousey - pmouseY);
	deltaMouse = deltaMouse * deltaTime;
	if (!ImGui::IsAnyItemActive())
	{
		if (glfwGetMouseButton(window, 0))
			cam.LookAround(deltaMouse);
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			glm::vec3 right = glm::vec3(cam.m_view[0][0], cam.m_view[1][0], cam.m_view[2][0]);
			cam.setPosition(right * deltaTime * timeHeld);
			timeHeld += deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_W))
		{
			glm::vec3 forward = glm::vec3(cam.m_view[0][2], cam.m_view[1][2], cam.m_view[2][2]);
			cam.setPosition(-forward* deltaTime* timeHeld);
			timeHeld += deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_S))
		{
			glm::vec3 forward = glm::vec3(cam.m_view[0][2], cam.m_view[1][2], cam.m_view[2][2]);
			cam.setPosition(forward* deltaTime* timeHeld);
			timeHeld += deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_A))
		{
			glm::vec3 right = glm::vec3(cam.m_view[0][0], cam.m_view[1][0], cam.m_view[2][0]);
			cam.setPosition(-right* deltaTime*timeHeld);
			timeHeld += deltaTime;
		}
	}
	if (glm::length(cam.m_position - oldCam) < .01)
		timeHeld = 1;


	pmouseX = mousex;
	pmouseY = mousey;
	return true;
}

bool TextureApp::Shutdown()
{
	return true;
}

bool TextureApp::Draw()
{
	glUseProgram(shader);
	ImGui_ImplGlfwGL3_NewFrame();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	unsigned int pvm = vert.getUniform("ProjectionViewModel");
	unsigned int time = glGetUniformLocation(shader, "time");
	glUniform1f(time, glfwGetTime());
	glUniformMatrix4fv(pvm, 1, false, glm::value_ptr(cam.getProjectionView()));
	unsigned int texID = frag.getUniform("tex");
	glUniform1i(texID, 0);
	unsigned int texV = vert.getUniform("tex");
	glUniform1i(texV, 0);

	ImGui::Begin("test");
	ImGui::SetWindowSize(ImVec2(200, 200));
	if (selectedMesh != nullptr)
		ImGui::InputText("Texture File", selectedMesh->texture.textureFile, 50);
	ImGui::End();

	ImGui::Begin("test move");
	ImGui::SetWindowSize(ImVec2(500, 500));
	int i = 0;
	for (auto m : objects)
	{
		std::string s = std::to_string(i);
		s = "geometry " + s;
		char const *pchar = s.c_str();
		if (ImGui::Button(pchar, ImVec2(70, 20)))
			selectedMesh = &objects[i];
		i++;
	}
	if (selectedMesh != nullptr)
	{
		ImGui::DragFloat("X", &(selectedMesh->m_position.x), .05f, -9000, 9000);
		ImGui::DragFloat("Y", &(selectedMesh->m_position.y), .05f, -9000, 9000);
		ImGui::DragFloat("Z", &(selectedMesh->m_position.z), .05f, -9000, 9000);
	}
	ImGui::End();
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
	glUniformMatrix4fv(pvm, 1, false, glm::value_ptr(cam.getProjectionView() * glm::scale(glm::vec3(5))));
	int lightUniform = frag.getUniform("direction");
	glUniform3fv(lightUniform, 1, glm::value_ptr(m_directLight.direction));

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
	glUniform3fv(lightUniform, 1, glm::value_ptr(cam.m_position));


	int rnad = vert.getUniform("rand");
	float rnd = rand() % 60 + 30;
	glUniform1f(rnad, rnd);
	for (auto m : objects)
	{

		unsigned int pvm = vert.getUniform("ProjectionViewModel");
		glUniformMatrix4fv(pvm, 1, false, glm::value_ptr(cam.getProjectionView() * glm::scale(glm::vec3(5)) *glm::translate(m.m_position)));

		m.bind();
		m.draw();
		m.unbind();
	}

	glUseProgram(0);

	ImGui::Render();
	return true;
}
Mesh TextureApp::GenGrid(unsigned int rows, unsigned int cols)
{
	Vertex * aoVerts = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
		{
			Vertex v = { glm::vec4(float(c), 0, float(r), 1),glm::vec4(sin(r),cos(c),0,1),glm::vec4(0,1,0,0),glm::vec2(float(c) / float(cols - 1),float(r) / float(rows - 1)) };
			//v.color = glm::vec4(1);
			aoVerts[r * cols + c] = v;
			//glm::vec3 color = glm::vec3(sinf( 12 * cosf(10 * tanf(float(c) / (float(cols))))), cosf(9 * sinf(21 * tanf(float(c) / (float(cols))))), tanf(34 * cosf(24 * sinf(float(c) / (float(cols))))));
			//glm::vec3 color = glm::vec3((float)r / (float)rows, ((float)r / (float)rows) * ((float)c / (float)cols), (float)c / (float)cols);
			//aoVerts[r * cols + c].color = glm::vec4(color, 1);
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
	Mesh gridMesh = Mesh();
	gridMesh.initialize(verts, indices, GL_TRIANGLES);
	verts.clear();
	indices.clear();
	delete[] auiIndices;
	delete[] aoVerts;
	return gridMesh;
}



void generateSphere(unsigned int segments, unsigned int rings,
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