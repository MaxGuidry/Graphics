#include "DollyCamera.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.inl>
#include <glfw/glfw3.h>

DollyCamera::DollyCamera() : m_position(10, 10, 10), m_view(glm::mat4(1)), m_fov(glm::pi<float>() / 4.f), m_aspectRatio(16.f / 9.f), m_near(.1f), m_far(1000.f), worldTransform(glm::mat4(1)), m_projection(glm::mat4(1)), viewTransform(glm::mat4(1)), projectionTransform(glm::mat4(1)), projectionViewTransform(glm::mat4(1)), focus(0, 0, 0)
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

void DollyCamera::RotateAround(glm::vec2 deltaMouse)
{
	float mag = glm::sqrt(m_position.x * m_position.x + m_position.y * m_position.y + m_position.z * m_position.z);
	glm::vec3 mouse3(deltaMouse.x, deltaMouse.y, 0);
	float xmag = glm::sqrt(mouse3.x * mouse3.x);
	float ymag = glm::sqrt(mouse3.y * mouse3.y);
	glm::vec3 right(getView()[0][0], getView()[1][0], getView()[2][0]);
	float absx = glm::abs(deltaMouse.x);
	float absy = glm::abs(deltaMouse.y);
	if (absx == 0 && absy == 0)
		return;
	if (absx != 0)
		xmag = (xmag * -deltaMouse.x) / absx;
	if (absy != 0)
		ymag = (ymag * deltaMouse.y) / absy;
	right = right * xmag;
	glm::vec3 up(getView()[0][1], getView()[1][1], getView()[2][1]);
	up = up * ymag;
	m_position += right;
	m_position += up;
	m_position = glm::normalize(m_position) * mag;
	glm::vec3 wup(0, 1, 0);
	LookAt(m_position, focus, wup);
}

void DollyCamera::LookAround(glm::vec2 deltaMouse)
{
	glm::mat4 rotx =
		glm::mat4(
			1, cos(deltaMouse.y), -sin(deltaMouse.y), 0,
			0, sin(deltaMouse.y), cos(deltaMouse.y), 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	glm::mat4 roty =
		glm::mat4(
			cos(deltaMouse.x), 0, sin(deltaMouse.x), 0,
			0, 1, 0, 0,
			-sin(deltaMouse.x), 0, cos(deltaMouse.x), 0,
			0, 0, 0, 1
		);

}

void DollyCamera::updateProjectionViewTransfrom()
{
}
