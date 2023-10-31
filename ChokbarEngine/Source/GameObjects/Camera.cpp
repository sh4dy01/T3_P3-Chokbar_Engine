#include "Chokbar.h"
#include "GameObjects/Camera.h"

Camera::Camera()
	: m_CameraComponent(nullptr)
{
	m_CameraComponent = AddComponent<CameraComponent>();
	m_PlayerComponent = AddComponent<PlayerComponent>();
}

Camera::~Camera()
{
	delete m_CameraComponent;
	m_CameraComponent = nullptr;

	delete m_PlayerComponent;
	m_PlayerComponent = nullptr;
}
