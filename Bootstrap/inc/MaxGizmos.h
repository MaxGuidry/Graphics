#pragma once
#include "glm/glm.hpp"
#include "Mesh.h"
#include <glm/ext.hpp>

class MaxGizmos
{
public:
	MaxGizmos();
	~MaxGizmos();

	
	
	static Mesh GenSphere(float radius, unsigned int circleSize, unsigned int meridians);
private:
	static std::vector<glm::vec4> HalfCircle(float radius, unsigned int size);
	static std::vector<glm::vec4> GenSphereVerts(std::vector<glm::vec4> halfCircle, unsigned int meridians);
	static std::vector<unsigned int> GenSphereIndices(unsigned int size, unsigned int meridians);
};

