#include "DollyCamera.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.inl>
#include<cmath>

DollyCamera::DollyCamera() : m_position(glm::mat4(1)), m_view(glm::mat4(1)), m_fov(glm::pi<float>() / 4.f), m_aspectRatio(16.f / 9.f), m_near(.1f), m_far(1000.f), worldTransform(glm::mat4(1)), m_projection(glm::mat4(1)), viewTransform(glm::mat4(1)), projectionTransform(glm::mat4(1)), projectionViewTransform(glm::mat4(1))
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
	auto x = 1.f / (m_aspectRatio * tan(m_fov / 2.f));
	auto y = 1.f / ( tan(m_fov / 2.f));
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
	glm::vec3 z = glm::normalize((eye - center));
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
	return glm::mat4(0);

}

glm::mat4 DollyCamera::getView()
{
	return glm::mat4(0);

}

glm::mat4 DollyCamera::getProjection()
{
	return glm::mat4(0);

}

glm::mat4 DollyCamera::getProjectionView()
{
	/*glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::pi<float>() * .25f, 16.0f / 9.0f, .1f, 1000.f);
	glm::mat4 projtview = proj * view;*/
	return m_projection * m_view;
}

void DollyCamera::updateProjectionViewTransfrom()
{
}
