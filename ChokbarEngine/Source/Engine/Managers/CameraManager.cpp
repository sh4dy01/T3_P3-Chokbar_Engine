#include "Chokbar.h"
#include "Engine/Managers/CameraManager.h"

CameraManager::CameraManager()
	: m_MainCamera(nullptr)
{
}

CameraManager::~CameraManager()
{
	m_MainCamera = nullptr;
}

void CameraManager::CreateDefaultCamera()
{
	const auto mainCamera = new Camera("MainCamera");
	mainCamera->transform->SetPosition(0, 2, -5);
	//mainCamera->GetCameraComponent()->LookAt({ 0, 0, 0 });

	SetMainCamera(mainCamera->GetCameraComponent());
}

void CameraManager::SetMainCamera(CameraComponent* camera)
{
	m_MainCamera = camera;
}
