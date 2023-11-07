#pragma once

class PlayerShoot : public CustomComponent
{
public:

	PlayerShoot() = default;

	void Awake() override;
	void Start() override;
	void Update() override;

private:

	void HandleShoot();
	void ShootProjectileFromWings();
	void ShootProjectile(XMFLOAT3 position, XMFLOAT3 direction);

	void HandleZoomAndSlowMotion();

private:

	const float m_ShootOffset = 2.0f;
	const float m_ShootDelay = 0.1f;
	float m_ShootTimer = 0.0f;

	const float m_ProjectileSpeed = 10.0f;
	const float m_ProjectileLifeTime = 2.0f;

	const float m_ZoomSpeed = 0.1f;
	const float m_Zoom = 2.0f;

	const float m_BasicFOV = 70;

	const float m_SlowMotionRecoverSpeed = 1.f;
	const float m_SlowMotionSpeed = 0.5f;
	const float m_SlowMotion = 0.1f;

	CameraComponent* m_pCamera;
	Rigidbody* m_pRigidbody;
};