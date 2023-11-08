#include "BlankProject.h"
#include "PlayerMovement.h"

void PlayerMovement::Awake()
{
	m_CurrentVerticalThrust = 0.0f;
	m_CurrentForwardThrust = 0.0f;
	m_CurrentLateralThrust = 0.0f;
}

void PlayerMovement::Start()
{
	m_pRigidbody = gameObject->GetComponent<Rigidbody>();
}

void PlayerMovement::Update()
{
	HandleForwardThrust();
	HandleLateralThrust();
	HandleVerticalThrust();

	HandleRotation();
}

void PlayerMovement::FixedUpdate()
{
	ApplyMovement();
}


void PlayerMovement::HandleForwardThrust()
{
	if (InputHandler::IsKeyHeld('z'))
	{
		if (m_CurrentForwardThrust < 0)
		{
			ApplyOppositeForce(m_CurrentForwardThrust, m_ForwardThrustIncrement);
		}
		else
		{
			m_CurrentForwardThrust += m_ForwardThrustIncrement;
		}
	}
	else if (InputHandler::IsKeyHeld('s'))
	{
		if (m_CurrentForwardThrust > 0)
		{
			ApplyOppositeForce(m_CurrentForwardThrust, -m_ForwardThrustIncrement);
		}
		else
		{
			m_CurrentForwardThrust -= m_ForwardThrustIncrement;
		}
	}
	else
	{
		m_CurrentForwardThrust *= m_ForwardThrustReduction;
	}
}
void PlayerMovement::HandleLateralThrust()
{
	if (InputHandler::IsKeyHeld('d'))
	{
		if (m_CurrentLateralThrust < 0)
		{
			ApplyOppositeForce(m_CurrentLateralThrust, m_LateralThrustIncrement);
		}
		else
		{
			m_CurrentLateralThrust += m_LateralThrustIncrement;
		}
	}
	else if (InputHandler::IsKeyHeld('q'))
	{
		if (m_CurrentLateralThrust > 0)
		{
			ApplyOppositeForce(m_CurrentLateralThrust, -m_LateralThrustIncrement);
		}
		else
		{
			m_CurrentLateralThrust -= m_LateralThrustIncrement;
		}
	}
	else
	{
		m_CurrentLateralThrust *= m_LateralThrustReduction;
	}
}
void PlayerMovement::HandleVerticalThrust()
{
	if (InputHandler::IsKeyHeld(VK_SHIFT))
	{
		m_CurrentVerticalThrust -= m_VerticalThrustIncrement;
	}
	else if (InputHandler::IsKeyHeld(VK_SPACE))
	{
		m_CurrentVerticalThrust += m_VerticalThrustIncrement;
	}
	else
	{
		m_CurrentVerticalThrust *= m_VerticalThrustReduction;
	}
}

void PlayerMovement::ApplyOppositeForce(float& outForce, float incrementValue)
{
	outForce += incrementValue * m_OppositeThrustMultiplier;
}

void PlayerMovement::ApplyMovement()
{
	m_pRigidbody->AddVelocity(
		std::clamp(m_CurrentLateralThrust, -m_MaxLateralThrust, m_MaxLateralThrust) * TimeManager::GetDeltaTime(),
		std::clamp(m_CurrentVerticalThrust, -m_MaxVerticalThrust, m_MaxVerticalThrust) * TimeManager::GetDeltaTime(),
		std::clamp(m_CurrentForwardThrust, -m_MaxForwardThrust, m_MaxForwardThrust) * TimeManager::GetDeltaTime());
}

void PlayerMovement::HandleRotation()
{
	transform->RotatePitch(InputHandler::GetAxisY() * m_PitchTorque * m_Sensitivity * TimeManager::GetDeltaTime());
	transform->RotateYaw(InputHandler::GetAxisX() * m_YawTorque * m_Sensitivity * TimeManager::GetDeltaTime());

	if (InputHandler::IsKeyHeld('a'))
	{
		transform->RotateRoll(m_RollTorque * m_Sensitivity * TimeManager::GetDeltaTime());
	}
	else if (InputHandler::IsKeyHeld('e'))
	{
		transform->RotateRoll(-m_RollTorque * m_Sensitivity * TimeManager::GetDeltaTime());
	}
}