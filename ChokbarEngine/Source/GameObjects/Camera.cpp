#include "Chokbar.h"
#include "GameObjects/Camera.h"

Camera::Camera(const std::string& name)
	: GameObject(name), m_CameraComponent(AddComponent<CameraComponent>()), m_PlayerComponent(AddComponent<PlayerComponent>())
{
	AddComponent<Rigidbody>();
	AddComponent<SphereCollider>();

	transform->SetPosition(0, 0, -2);
}

Camera::~Camera()
{
	delete m_CameraComponent;
	m_CameraComponent = nullptr;

	delete m_PlayerComponent;
	m_PlayerComponent = nullptr;
}
