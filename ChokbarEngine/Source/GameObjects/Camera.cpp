#include "Chokbar.h"
#include "GameObjects/Camera.h"

Camera::Camera()
	: m_CameraComponent(nullptr)
{
	m_CameraComponent = AddComponent<CameraComponent>();
	m_PlayerComponent = AddComponent<PlayerComponent>();
	m_Rigidbody = AddComponent<Rigidbody>(new Rigidbody());
	m_Sphere = AddComponent<Sphere>(new Sphere({ 0, 0, 0 }, 2));
	transform->SetPosition(0, 0, -2);
	m_CameraComponent->LookAt(transform->GetPosition(), {0, 0, 0}, {0, 1, 0});
	transform->Rotate(0, 0, 90);
}

Camera::~Camera()
{
	delete m_CameraComponent;
	m_CameraComponent = nullptr;

	delete m_PlayerComponent;
	m_PlayerComponent = nullptr;
}
