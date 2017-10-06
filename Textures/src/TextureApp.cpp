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

unsigned int getRandom(unsigned seed0, unsigned seed1)
{
	for (int x = 0; x < 50; x++)
		for (int y = 0; y < 50; y++)
			std::cout << (x&y) << std::endl;

	seed1 = 36969 * (seed1 & 65535) + (seed1 >> 16);
	seed0 = 18000 * (seed0 & 65535) + (seed0 >> 16);
	return (seed1 << 16) + seed0;
}

float maxNoise(glm::vec2 verts, unsigned int dims, int seed)
{


	int prime = 36343;
	if (seed == 0)
		seed = prime;
	if (dims == 0)
		dims = prime;
	
	char * p = new char[1];
	p++;
	while (p == nullptr || *p == 0)
	{
		p = new char[1];
		p++;
	}
	float c = *p;
	c *= sqrt(seed);
	
		//(prime / seed) << dims / prime;
	
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
	glm::vec4 lerped = glm::vec4(glm::lerp(glm::vec2(x, y), glm::vec2(x2, y), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x, y2), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x2, y2), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x, y3), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x3, y), .1f) + glm::lerp(glm::vec2(x, y), glm::vec2(x3, y3), .1f), 1, 1);
	/*if (seed > prime)
		lerped = lerped * glm::rotate(float(sqrt(abs(c))), glm::vec3(numa, numb, glm::fract(prime / c)));*/
		//if (seed > dims)
		//	lerped = lerped * glm::rotate(float(sqrt(abs(prime))), glm::vec3(c, prime, glm::fract(numb)));
	lerped = lerped * glm::rotate(float(sqrt(abs(c))), glm::vec3(sqrt(float(seed)), c, numb *numa));
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

int a[] = { 27697,	27701,	27733,	27737,	27739,	27743,	27749,	27751,	27763,	27767,
27773,	27779,	27791,	27793,	27799,	27803,	27809,	27817,	27823,	27827,
27847,	27851,	27883,	27893,	27901,	27917,	27919,	27941,	27943,	27947,
27953,	27961,	27967,	27983,	27997,	28001,	28019,	28027,	28031,	28051,
28057,	28069,	28081,	28087,	28097,	28099,	28109,	28111,	28123,	28151,
28163,	28181,	28183,	28201,	28211,	28219,	28229,	28277,	28279,	28283,
28289,	28297,	28307,	28309,	28319,	28349,	28351,	28387,	28393,	28403,
28409,	28411,	28429,	28433,	28439,	28447,	28463,	28477,	28493,	28499,
28513,	28517,	28537,	28541,	28547,	28549,	28559,	28571,	28573,	28579,
28591,	28597,	28603,	28607,	28619,	28621,	28627,	28631,	28643,	28649,
28657,	28661,	28663,	28669,	28687,	28697,	28703,	28711,	28723,	28729,
28751,	28753,	28759,	28771,	28789,	28793,	28807,	28813,	28817,	28837,
28843,	28859,	28867,	28871,	28879,	28901,	28909,	28921,	28927,	28933,
28949,	28961,	28979,	29009,	29017,	29021,	29023,	29027,	29033,	29059,
29063,	29077,	29101,	29123,	29129,	29131,	29137,	29147,	29153,	29167,
29173,	29179,	29191,	29201,	29207,	29209,	29221,	29231,	29243,	29251,
29269,	29287,	29297,	29303,	29311,	29327,	29333,	29339,	29347,	29363,
29383,	29387,	29389,	29399,	29401,	29411,	29423,	29429,	29437,	29443,
29453,	29473,	29483,	29501,	29527,	29531,	29537,	29567,	29569,	29573,
29581,	29587,	29599,	29611,	29629,	29633,	29641,	29663,	29669,	29671,
29683,	29717,	29723,	29741,	29753,	29759,	29761,	29789,	29803,	29819,
29833,	29837,	29851,	29863,	29867,	29873,	29879,	29881,	29917,	29921,
29927,	29947,	29959,	29983,	29989,	30011,	30013,	30029,	30047,	30059,
30071,	30089,	30091,	30097,	30103,	30109,	30113,	30119,	30133,	30137,
30139,	30161,	30169,	30181,	30187,	30197,	30203,	30211,	30223,	30241,
30253,	30259,	30269,	30271,	30293,	30307,	30313,	30319,	30323,	30341,
30347,	30367,	30389,	30391,	30403,	30427,	30431,	30449,	30467,	30469,
30491,	30493,	30497,	30509,	30517,	30529,	30539,	30553,	30557,	30559,
30577,	30593,	30631,	30637,	30643,	30649,	30661,	30671,	30677,	30689,
30697,	30703,	30707,	30713,	30727,	30757,	30763,	30773,	30781,	30803,
30809,	30817,	30829,	30839,	30841,	30851,	30853,	30859,	30869,	30871,
30881,	30893,	30911,	30931,	30937,	30941,	30949,	30971,	30977,	30983,
31013,	31019,	31033,	31039,	31051,	31063,	31069,	31079,	31081,	31091,
31121,	31123,	31139,	31147,	31151,	31153,	31159,	31177,	31181,	31183,
31189,	31193,	31219,	31223,	31231,	31237,	31247,	31249,	31253,	31259,
31267,	31271,	31277,	31307,	31319,	31321,	31327,	31333,	31337,	31357,
31379,	31387,	31391,	31393,	31397,	31469,	31477,	31481,	31489,	31511,
31513,	31517,	31531,	31541,	31543,	31547,	31567,	31573,	31583,	31601,
31607,	31627,	31643,	31649,	31657,	31663,	31667,	31687,	31699,	31721,
31723,	31727,	31729,	31741,	31751,	31769,	31771,	31793,	31799,	31817,
31847,	31849,	31859,	31873,	31883,	31891,	31907,	31957,	31963,	31973,
31981,	31991,	32003,	32009,	32027,	32029,	32051,	32057,	32059,	32063,
32069,	32077,	32083,	32089,	32099,	32117,	32119,	32141,	32143,	32159,
32173,	32183,	32189,	32191,	32203,	32213,	32233,	32237,	32251,	32257,
32261,	32297,	32299,	32303,	32309,	32321,	32323,	32327,	32341,	32353,
32359,	32363,	32369,	32371,	32377,	32381,	32401,	32411,	32413,	32423,
32429,	32441,	32443,	32467,	32479,	32491,	32497,	32503,	32507,	32531,
32533,	32537,	32561,	32563,	32569,	32573,	32579,	32587,	32603,	32609,
32611,	32621,	32633,	32647,	32653,	32687,	32693,	32707,	32713,	32717,
32719,	32749,	32771,	32779,	32783,	32789,	32797,	32801,	32803,	32831,
32833,	32839,	32843,	32869,	32887,	32909,	32911,	32917,	32933,	32939,
32941,	32957,	32969,	32971,	32983,	32987,	32993,	32999,	33013,	33023,
33029,	33037,	33049,	33053,	33071,	33073,	33083,	33091,	33107,	33113,
33119,	33149,	33151,	33161,	33179,	33181,	33191,	33199,	33203,	33211,
33223,	33247,	33287,	33289,	33301,	33311,	33317,	33329,	33331,	33343,
33347,	33349,	33353,	33359,	33377,	33391,	33403,	33409,	33413,	33427,
33457,	33461,	33469,	33479,	33487,	33493,	33503,	33521,	33529,	33533 };
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
	int seed = 7;
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




				seed = a[rand() % 569];
				//std::cout << seed << std::endl;
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
			if (f > dims && f < (dims * dims) - 1)
				perlinData[f] = (perlinData[f] + perlinData[f-dims]) / 2.f;
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