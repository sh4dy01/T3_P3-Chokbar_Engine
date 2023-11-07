#include "BlankProject.h"
#include "PlayerComponent.h"

void PlayerComponent::Awake()
{
	m_Speed = 10.f;

	m_pRigidbody = gameObject->GetComponent<Rigidbody>();
}

void PlayerComponent::Start()
{
	m_pCamera = CameraManager::GetMainCamera();
}

void PlayerComponent::Update()
{
	if (InputHandler::IsKeyHeld('z'))
	{
		m_pRigidbody->Move(0, 0, m_Speed * TimeManager::GetDeltaTime());
	}
	if (InputHandler::IsKeyHeld('s'))
	{
		m_pRigidbody->Move(0, 0, -m_Speed * TimeManager::GetDeltaTime());
	}
	if (InputHandler::IsKeyHeld('q'))
	{
		m_pRigidbody->Move(-m_Speed * TimeManager::GetDeltaTime(), 0, 0);
	}
	if (InputHandler::IsKeyHeld('d'))
	{
		m_pRigidbody->Move(m_Speed * TimeManager::GetDeltaTime(), 0, 0);
	}
	if (InputHandler::IsKeyDown(VK_RBUTTON))
	{
		//m_pCamera->SetFOV(10.f);
		TimeManager::SetTimeScale(0.1f);
	}
	else if (InputHandler::IsKeyUp(VK_RBUTTON))
	{
		//m_pCamera->SetFOV(75.f);
		TimeManager::SetTimeScale(1.f);
	}
	if (InputHandler::IsKeyHeld(VK_SHIFT))
	{
		m_pRigidbody->Move(0, -m_Speed * TimeManager::GetDeltaTime(), 0, Transform::Space::World);
	}
	if (InputHandler::IsKeyHeld(VK_SPACE))
	{
		m_pRigidbody->Move(0, m_Speed * TimeManager::GetDeltaTime(), 0, Transform::Space::World);
	}

	transform->RotateYaw(InputHandler::GetAxisX() * 50.f * TimeManager::GetDeltaTime(), Transform::Space::World);

	transform->RotatePitch(InputHandler::GetAxisY() * 50.f * TimeManager::GetDeltaTime());
}
