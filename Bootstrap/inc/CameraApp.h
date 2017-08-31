#pragma once
#include "Application.h"
#include "DollyCamera.h"

class CameraApp :
	public Application
{
public:
	CameraApp();
	~CameraApp();
protected:
	bool Start() override;
	bool Update(float deltaTime) override;
	bool Shutdown() override;
	bool Draw() override;
	DollyCamera * m_camera;
private:
	double pmouseX;
	double pmouseY;
};

