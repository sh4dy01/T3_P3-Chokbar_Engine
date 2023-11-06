#pragma once
#include "Engine/ECS/Components/CameraComponent.h"

class CameraManager
{
public :

	CameraManager();
	~CameraManager();

	static void SetMainCamera(CameraComponent* camera);
	static CameraComponent* GetMainCamera() { return m_MainCamera; }

private:


	static CameraComponent* m_MainCamera;

};