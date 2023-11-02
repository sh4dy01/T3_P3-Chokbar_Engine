#pragma once
#include "GameObjects/Camera.h"


class CameraManager
{
public :

	CameraManager();
	~CameraManager();


	void CreateDefaultCamera();


	void SetMainCamera(Camera* camera);
	Camera* GetMainCamera() const { return m_MainCamera; }

private:

	Camera* m_MainCamera;

};