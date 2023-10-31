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

void CameraManager::SetMainCamera(Camera* camera)
{
	m_MainCamera = camera;
}
