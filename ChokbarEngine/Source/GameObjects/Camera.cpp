#include "Chokbar.h"
#include "GameObjects/Camera.h"

Camera::Camera()
	: m_CameraComponent(nullptr)
{
	AddComponent<CameraComponent>();
	m_CameraComponent = GetComponent<CameraComponent>();
}

Camera::~Camera()
{
	m_CameraComponent = nullptr;
	delete m_CameraComponent;
}
