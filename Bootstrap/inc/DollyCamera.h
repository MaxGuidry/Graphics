#pragma once
#pragma once
#include <glm/glm.hpp>
class DollyCamera
{
public:
	DollyCamera();
	~DollyCamera();

	glm::vec3 m_position;
	glm::mat4 m_view;
	glm::vec3 focus;
	virtual void update(float deltaTime);
	void setPerspective(float FOV, float aspectRatio, float near, float far);
	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
	void setPosition(glm::vec3 position);
	glm::mat4 getWorldTransform();
	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();
	glm::vec3 RotateAround(glm::vec3 position, glm::vec3 orbitPos, glm::vec2 deltaMouse,glm::mat4 objMat);
	void LookAround(glm::vec2 deltaMouse);
	void SimpleRot(glm::vec2 deltaMouse);
private:
	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;

	glm::mat4 worldTransform;
	glm::mat4 m_projection;
	glm::mat4 projectionViewTransform;

	void updateProjectionViewTransfrom();
};
