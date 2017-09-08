#define GLM_FORCE_SWIZZLE
#include "DollyCamera.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include <iostream>

DollyCamera::DollyCamera() : m_position(10, 10, 10), m_view(glm::mat4(1)), focus(0, 0, 0), m_fov(glm::pi<float>() / 4.f), m_aspectRatio(16.f / 9.f), m_near(.1f), m_far(1000.f), worldTransform(glm::mat4(1)), m_projection(glm::mat4(1)), projectionViewTransform(glm::mat4(1))
{

	setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 0.1f, 1000.f);

}


DollyCamera::~DollyCamera()
{
}

void DollyCamera::update(float deltaTime)
{

}

void DollyCamera::setPerspective(float FOV, float aspectRatio, float near, float far)
{
	m_fov = FOV;
	m_aspectRatio = aspectRatio;
	m_near = near;
	m_far = far;
	auto x = 1.f / (m_aspectRatio * glm::tan(m_fov / 2.f));
	auto y = 1.f / (glm::tan(m_fov / 2.f));
	auto z = -1.f * ((m_far + m_near) / (m_far - m_near));
	auto w = -1.f * ((2.f * m_far * m_near) / (m_far - m_near));

	m_projection = glm::mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, -1,
		0, 0, w, 0);
	auto copy = glm::perspective(m_fov, aspectRatio, near, far);
	assert(copy == m_projection);
}

void DollyCamera::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	glm::vec3 z = glm::normalize(eye - center);
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::cross(z, x);
	glm::mat4 v = glm::mat4(
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		0, 0, 0, 1);


	glm::mat4 translation = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye.x, -eye.y, -eye.z, 1);
	glm::mat4 view = v * translation;
	m_view = view;
	glm::mat4 lookat = glm::lookAt(eye, center, up);
	assert(view == lookat);
	worldTransform = glm::inverse(view);


}

void DollyCamera::setPosition(glm::vec3 position)
{
	glm::mat4 t =
		glm::mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-position.x, -position.y, -position.z, 1
		);
	this->m_position = this->m_position + position;
	m_view = m_view * t;
	//m_position = glm::vec3(m_view[3][0], m_view[3][1], m_view[3][2]);
	worldTransform = glm::inverse(m_view);
}

glm::mat4 DollyCamera::getWorldTransform()
{
	return worldTransform;

}

glm::mat4 DollyCamera::getView()
{
	return m_view;

}

glm::mat4 DollyCamera::getProjection()
{
	return m_projection;

}

glm::mat4 DollyCamera::getProjectionView()
{

	return m_projection * m_view;
}

glm::vec3 DollyCamera::RotateAround(glm::vec3 position, glm::vec3 CenterOfOrbit, glm::vec2 deltaMouse, glm::mat4 objMat)
{
	glm::vec3 dist = position - CenterOfOrbit;
	float mag = glm::sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
	glm::vec3 mouse3(deltaMouse.x, deltaMouse.y, 0);
	float xmag = mouse3.x;
	float ymag = mouse3.y;
	glm::vec3 wup = glm::vec3(0, 1, 0);
	glm::vec3 right = glm::cross(glm::normalize(dist), wup);
	float absx = glm::abs(deltaMouse.x);
	float absy = glm::abs(deltaMouse.y);
	if (absx == 0 && absy == 0)
		return glm::vec3(0);
	if (absx != 0)
		xmag = (xmag * deltaMouse.x) / absx;
	if (absy != 0)
		ymag = (ymag * deltaMouse.y) / absy;
	right = right * xmag;
	glm::vec3 up = glm::cross(right, glm::normalize(dist));

	up = up * ymag;
	position += right;
	position += up;
	position = glm::normalize(position) * mag;
	return position;

}

void DollyCamera::LookAround(glm::vec2 deltaMouse)
{

	deltaMouse = -deltaMouse * .1f;
	glm::vec3 oldZAxis = glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]);


	float angley = glm::acos(glm::dot(glm::vec3(0, 0, 1), glm::normalize(glm::vec3(oldZAxis.x, 0, 1))));
	//float anglex = glm::acos(glm::dot(glm::vec3(0, 0, 1), glm::normalize(glm::vec3(0, oldZAxis.y, 1))));
	//glm::mat4 Y = glm::mat4(1);
	/*glm::mat4 xrotfory =
		glm::mat4(
			1, 0, 0, 0,
			0, glm::cos(-angley), -glm::sin(-angley), 0,
			0, glm::sin(-angley), glm::cos(-angley), 0,
			0, 0, 0, 1
		);
	glm::mat4 yrotfory =
		glm::mat4(
			glm::cos(-anglex), 0, glm::sin(-anglex), 0,
			0, 1, 0, 0,
			-glm::sin(-anglex), 0, glm::cos(-anglex), 0,
			0, 0, 0, 1
		);*/
		/*glm::mat4 combinedYROT = xrotfory * yrotfory;
		Y = combinedYROT *Y;
		glm::vec3 YAxis = glm::vec3(Y[0][1], Y[1][1], Y[2][1]);*/
	glm::mat4 rotx =
		glm::mat4(
			1, 0, 0, 0,
			0, glm::cos(deltaMouse.y), -glm::sin(deltaMouse.y), 0,
			0, glm::sin(deltaMouse.y), glm::cos(deltaMouse.y), 0,
			0, 0, 0, 1
		);
	glm::mat4 roty =
		glm::mat4(
			glm::cos(deltaMouse.x), 0, glm::sin(deltaMouse.x), 0,
			0, 1, 0, 0,
			-glm::sin(deltaMouse.x), 0, glm::cos(deltaMouse.x), 0,
			0, 0, 0, 1
		);



	//glm::mat4 correctz =
	//	glm::mat4(

	//		glm::cos(-angley), -glm::sin(-angley), 0, 0,
	//		glm::sin(-angley), glm::cos(-angley), 0, 0,
	//		0, 0, 1, 0,
	//		0, 0, 0, 1
	//	);
	//glm::mat4 correcty =
	//	glm::mat4(
	//		glm::cos(-angley), 0, glm::sin(-angley), 0,
	//		0, 1, 0, 0,
	//		-glm::sin(-angley), 0, glm::cos(-angley), 0,
	//		0, 0, 0, 1
	//	);
	//glm::mat4 combinedRot = rotx * roty  * correctz *correcty;

	m_view = roty *  rotx * m_view;


	glm::vec3 prevz = glm::vec3(m_view[2][2], m_view[1][2], m_view[2][2]);
	//m_view = roty * m_view;
	//m_view = combinedRot * m_view;
	glm::vec3 newXAxis = glm::vec3(m_view[0][0], m_view[1][0], m_view[2][0]);
	glm::vec3 newZAxis = glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]);
	glm::vec3 newYAxis = glm::cross(newZAxis, newXAxis);


	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 newz = glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]);
	glm::vec3 z = glm::normalize(glm::vec3(newz.x, prevz.y, newz.z));
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::cross(z, x);
	glm::mat4 v = glm::mat4(
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		0, 0, 0, 1);
	//m_view[3][0], m_view[3][1], m_view[3][2]
	glm::mat4 translation = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	m_view = v *translation;
	this->LookAt(m_position, m_position - glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]), glm::vec3(0, 1, 0));

	worldTransform = glm::inverse(m_view);
}

void DollyCamera::SimpleRot(glm::vec2 deltaMouse)
{
	float pitch = deltaMouse.y;
	float yaw = deltaMouse.x;
	if (pitch > 89.f)
		pitch = 89.f;
	if (yaw > 89.f)
		yaw = 89.f;


	float newX = glm::cos(yaw);
	float newY = glm::sin(pitch);

	glm::vec3 z = glm::normalize(glm::vec3(newX, newY, 1));
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 x = glm::cross(z, up);
	glm::vec3 y = glm::cross(z, x);
	glm::mat4 v =
		glm::mat4(
			x.x, y.x, z.x, 0,
			x.y, y.y, z.y, 0,
			x.z, y.z, z.z, 0,
			0, 0, 0, 1);
	glm::mat4 t =
		glm::mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-m_position.x, -m_position.y, -m_position.z, 1);
	m_view = v * t;
	worldTransform = glm::inverse(m_view);
}

void DollyCamera::updateProjectionViewTransfrom()
{
}
