#pragma once
#pragma once
#include <glm/glm.hpp>
class DollyCamera
{
public:
	DollyCamera();
	~DollyCamera();

	glm::mat4 m_position;
	glm::mat4 m_view;

	virtual void update(float deltaTime);
	void setPerspective(float FOV, float aspectRatio, float near, float far);
	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
	void setPosition(glm::vec3 position);
	glm::mat4 getWorldTransform();
	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();


private:
	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;

	glm::mat4 worldTransform;
	glm::mat4 m_projection;
	glm::mat4 viewTransform;
	glm::mat4 projectionTransform;
	glm::mat4 projectionViewTransform;

	void updateProjectionViewTransfrom();
};
