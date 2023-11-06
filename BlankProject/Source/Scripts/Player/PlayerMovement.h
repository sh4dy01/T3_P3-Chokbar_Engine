#pragma once

class PlayerMovement : public CustomComponent
{
public:
	
	PlayerMovement() = default;

	void Awake() override;
	void Start() override;
	void Update() override;

private:

	void HandleForwardThrust();
	void HandleLateralThrust();
	void HandleVerticalThrust();

	void ApplyOppositeForce(float& outForce, float incrementValue);
	void ApplyMovement();

	void HandleRotation();

private:

	const float m_Sensitivity = 2;

	const float m_MaxVerticalThrust = 10.f;
	const float m_MaxForwardThrust = 15.f;
	const float m_MaxLateralThrust = 15.f;

	const float m_VerticalThrustIncrement = 0.15f;
	const float m_ForwardThrustIncrement = 0.15f;
	const float m_LateralThrustIncrement = 0.15f;

	const float m_VerticalThrustReduction = 0.99f;
	const float m_ForwardThrustReduction = 0.99f;
	const float m_LateralThrustReduction = 0.99f;

	const float m_OppositeThrustMultiplier = 3.5f;

	const float m_PitchTorque = 400;
	const float m_YawTorque = 400;

	float m_CurrentForwardThrust = 0.0f;
	float m_CurrentLateralThrust = 0.0f;
	float m_CurrentVerticalThrust = 0.0f;


	Rigidbody* m_pRigidbody;

};