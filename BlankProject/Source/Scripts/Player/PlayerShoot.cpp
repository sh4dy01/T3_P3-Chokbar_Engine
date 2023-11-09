#include "PlayerShoot.h"

#include "Scripts/ProjectileBehavior.h"
#include "GameObjects/Projectile.h"


void PlayerShoot::Awake()
{
	m_pCamera = CameraManager::GetMainCamera();
	m_pRigidbody = gameObject->GetComponent<Rigidbody>();

	m_LeftWing = transform->GetChild("LeftWing");
	m_RightWing = transform->GetChild("RightWing");
}

void PlayerShoot::Start()
{
	m_ShootTimer = m_ShootDelay;
}


void PlayerShoot::Update()
{
	HandleShoot();
	HandleSlowMotion();
}

void PlayerShoot::HandleShoot()
{
	if (InputHandler::IsKeyHeld(VK_LBUTTON) || InputHandler::IsKeyDown(VK_LBUTTON))
	{
		if (m_ShootTimer >= m_ShootDelay)
		{
			ShootProjectileFromWings();
			m_ShootTimer = 0;
		}
	}

	if (m_ShootTimer < m_ShootDelay)
	{
		m_ShootTimer += TimeManager::GetDeltaTime();
	}
}

void PlayerShoot::ShootProjectileFromWings()
{
	XMFLOAT3 forward = m_pCamera->GetLook3f();
	float angle = XMConvertToRadians(m_ShootOffsetAngle);

	ShootProjectile(m_LeftWing->GetWorldPosition(), forward);

	forward = m_pCamera->GetLook3f();
	//forward.x += angle;
	ShootProjectile(m_RightWing->GetWorldPosition(), forward);
}

void PlayerShoot::ShootProjectile(XMFLOAT3 position, XMFLOAT3 direction)
{
	XMFLOAT3 playerVelocity = m_pRigidbody->GetVelocity();
	float playerSpeed = XMVector3Length(XMLoadFloat3(&playerVelocity)).m128_f32[0];

	//XMVECTOR test = XMVectorScale(XMVectorAdd(XMLoadFloat3(&direction), XMLoadFloat3(&playerVelocity)), 0.5f);
	//MFLOAT3 test2;
	//XMStoreFloat3(&test2, XMVector3Normalize(test));

	const auto projectile = GameObject::Instantiate<Projectile>()->GetComponent<ProjectileBehavior>();
	projectile->transform->SetPosition(position);
	projectile->Initialize(direction, m_ProjectileSpeed + playerSpeed, m_ProjectileLifeTime);
}


void PlayerShoot::HandleSlowMotion()
{
	if (InputHandler::IsKeyHeld(VK_RBUTTON))
	{
		float currentTimeScale = TimeManager::GetTimeScale();

		if (currentTimeScale > m_SlowMotion)
		{
			currentTimeScale -= m_SlowMotionSpeed * TimeManager::GetUnscaledDeltaTime();
			TimeManager::SetTimeScale(std::clamp(currentTimeScale, 0.1f, 1.f));
		}
	}
	else if (TimeManager::GetTimeScale() < 1)
	{
		TimeManager::SetTimeScale(std::clamp(TimeManager::GetTimeScale() + m_SlowMotionRecoverSpeed * TimeManager::GetUnscaledDeltaTime(), 0.1f, 1.f));
	}

}