#include "Chokbar.h"
#include "Engine/Managers/CameraManager.h"

CameraComponent* CameraManager::m_MainCamera = nullptr;

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
	m_MainCamera = nullptr;
}

void CameraManager::SetMainCamera(CameraComponent* camera)
{
	m_MainCamera = camera;
}
