#include "Chokbar.h"
#include "Engine/Managers/CameraManager.h"

CameraManager::CameraManager()
	: m_MainCamera(nullptr)
{
}

CameraManager::~CameraManager()
{
	m_MainCamera = nullptr;
	delete m_MainCamera;
}

void CameraManager::CreateDefaultCamera()
{
	const auto mainCamera = new Camera();
	mainCamera->transform->SetPosition(0, 2, -5);
	//mainCamera->GetCameraComponent()->LookAt({ 0, 0, 0 });

	SetMainCamera(mainCamera);
}

void CameraManager::SetMainCamera(Camera* camera)
{
	m_MainCamera = camera;
}
