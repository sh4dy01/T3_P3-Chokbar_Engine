#pragma once
#include "GameObjects/Camera.h"


class CameraManager
{
public :

	CameraManager();
	~CameraManager();


	void CreateDefaultCamera();


	void SetMainCamera(CameraComponent* camera);
	CameraComponent* GetMainCamera() const { return m_MainCamera; }

private:

	CameraComponent* m_MainCamera;

};