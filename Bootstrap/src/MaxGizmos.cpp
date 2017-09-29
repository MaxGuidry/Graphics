#include "MaxGizmos.h"


MaxGizmos::MaxGizmos()
{
}


MaxGizmos::~MaxGizmos()
{
}
std::vector<glm::vec4> MaxGizmos::HalfCircle(float radius, unsigned int size)
{
	std::vector<glm::vec4> circleverts = std::vector<glm::vec4>();

	for (float i = 0; i < size; i++)
	{
		float x = radius *glm::cos((i*glm::pi<float>()) / ((float)size - 1));
		float y = radius *glm::sin((i*glm::pi<float>()) / ((float)size - 1));

		circleverts.push_back(glm::vec4(x, y, 0, 1));
	}
	return circleverts;
}
std::vector<glm::vec4> MaxGizmos::GenSphereVerts(std::vector<glm::vec4> halfCircle, unsigned int meridians)
{
	std::vector<glm::vec4> sphereVerts = std::vector<glm::vec4>();

	float phi = glm::pi<float>() / (((float)meridians / 2.f));
	float j = 0;
	for (int i = 0; i < meridians; i++)
	{

		for (auto v : halfCircle)
		{
			glm::vec4 p = glm::vec4();
			/*	p.position = v.position * glm::mat4(
			1, 0, 0, 0,
			0, glm::cos(j*phi), glm::sin(j*phi), 0,
			0, -glm::sin(j*phi), glm::cos(j*phi), 0,
			0, 0, 0, 1);*/
			p.x = v.x;
			p.y = v.y * glm::cos(i*phi) + v.z * -glm::sin(i*phi);
			p.z = v.y * glm::sin(i*phi) + v.z * glm::cos(i*phi);
			p.w = 1.f;

			sphereVerts.push_back(p);
		}
		j++;
	}

	for (int i = 0; i < 1; i++)
	{

		for (auto v : halfCircle)
		{
			glm::vec4 p = glm::vec4();
			/*	p.position = v.position * glm::mat4(
			1, 0, 0, 0,
			0, glm::cos(j*phi), glm::sin(j*phi), 0,
			0, -glm::sin(j*phi), glm::cos(j*phi), 0,
			0, 0, 0, 1);*/
			p.x = v.x;
			p.y = v.y * glm::cos(i*phi) + v.z * -glm::sin(i*phi);
			p.z = v.y * glm::sin(i*phi) + v.z * glm::cos(i*phi);
			p.w = 1.f;

			sphereVerts.push_back(p);
		}

	}

	return sphereVerts;



}
std::vector<unsigned int> MaxGizmos::GenSphereIndices(unsigned int size, unsigned int meridians)
{
	std::vector<unsigned int> sphereIndices = std::vector < unsigned int>();

	for (int i = 0; i < meridians; i++)
	{
		float botLeft = i*size;
		for (int j = 0; j < size; j++)
		{
			unsigned int left = j + botLeft;
			unsigned int right = left + size;

			sphereIndices.push_back(left);
			sphereIndices.push_back(right);
		}
		sphereIndices.push_back(0xFFFF);
	}
	return sphereIndices;
}
std::vector<glm::vec2> MaxGizmos::GenSphereUV(unsigned int circlesize, unsigned int meridians)
{
	std::vector<glm::vec2> uvs = std::vector<glm::vec2>();
	for (unsigned int segment = 0; segment < meridians; ++segment)
	{
		for (unsigned int ring = 0; ring < circlesize; ++ring)
		{
			uvs.push_back(glm::vec2(segment / (float)(meridians), ring / (float)(circlesize + 1)));
		}

	}

	for (unsigned int ring = 0; ring < circlesize; ++ring)
	{
		uvs.push_back(glm::vec2(meridians / (float)(meridians), ring / (float)(circlesize + 1)));
	}
	

	return uvs;
}


Mesh MaxGizmos::GenSphere(float radius, unsigned int circleSize, unsigned int meridians)
{

	std::vector<Vertex> sphereverts = std::vector<Vertex>();
	std::vector<glm::vec4> hc = HalfCircle(radius, circleSize);
	std::vector<glm::vec4> positions = GenSphereVerts(hc, meridians);
	std::vector<unsigned int> indices = GenSphereIndices(circleSize, meridians);
	std::vector<glm::vec2> uvs = GenSphereUV(circleSize, meridians);

	Mesh s = Mesh();
	unsigned int uvindex = 0;
	for (auto v : positions)
	{
		Vertex vert = Vertex();
		vert.position = v *  glm::mat4(
			glm::cos(glm::pi<float>()/2.f), -glm::sin(glm::pi<float>() / 2.f), 0, 0,
			glm::sin(glm::pi<float>() / 2.f), glm::cos(glm::pi<float>() / 2.f), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);;
		vert.color = v;
		vert.normal = glm::normalize(glm::vec4(v.x, v.y, v.z, 0));
		vert.tangent = glm::vec4(glm::cross(glm::vec3(0, 1, 0), glm::vec3(vert.normal.x, vert.normal.y, vert.normal.z)), 1);
		vert.texcoord = uvs[uvindex];
		uvindex++;
		sphereverts.push_back(vert);
	}
	s.initialize(sphereverts, indices, GL_TRIANGLE_STRIP);
	return s;
}