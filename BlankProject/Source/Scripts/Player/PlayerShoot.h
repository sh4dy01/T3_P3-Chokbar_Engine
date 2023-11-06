#pragma once

class PlayerShoot : public CustomComponent
{
public:

	PlayerShoot() = default;

	void Awake() override;
	void Start() override;
	void Update() override;

private:

	void HandleZoomAndSlowMotion();

private:

	const float m_ZoomSpeed = 0.1f;
	const float m_Zoom = 2.0f;

	const float m_BasicFOV = 70;

	const float m_SlowMotionRecoverSpeed = 0.7f;
	const float m_SlowMotionSpeed = 0.5f;
	const float m_SlowMotion = 0.1f;

	CameraComponent* m_pCamera;
	Rigidbody* m_pRigidbody;
};