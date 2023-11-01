#include "Chokbar.h"
#include "GameObjects/Camera.h"


Camera::Camera()
	: GameObject("camera"), m_CameraComponent(AddComponent<CameraComponent>()), m_PlayerComponent(AddComponent<PlayerComponent>())
{
	m_CameraComponent->LookAt(transform->GetPosition());
	//transform->Rotate(0, 0, 90);
}

Camera::~Camera()
{
	delete m_CameraComponent;
	m_CameraComponent = nullptr;

	delete m_PlayerComponent;
	m_PlayerComponent = nullptr;
}
