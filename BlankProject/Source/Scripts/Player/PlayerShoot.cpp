#include "PlayerShoot.h"

#include "Scripts/ProjectileBehavior.h"
#include "GameObjects/Projectile.h"


void PlayerShoot::Awake()
{
	m_pCamera = CameraManager::GetMainCamera();
	m_pRigidbody = gameObject->GetComponent<Rigidbody>();
}

void PlayerShoot::Start()
{
	m_pCamera->SetFOV(m_BasicFOV);
	m_ShootTimer = m_ShootDelay;
}


void PlayerShoot::Update()
{
	HandleShoot();
	HandleZoomAndSlowMotion();
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
	XMFLOAT3 projLeftPos = transform->GetPosition();
	XMStoreFloat3(&projLeftPos, XMVectorAdd(XMLoadFloat3(&projLeftPos), m_pCamera->GetLook()));
	projLeftPos.x -= m_ShootOffset;

	ShootProjectile(projLeftPos, m_pCamera->GetLook3f());


	XMFLOAT3 projRightPos = transform->GetPosition();
	XMStoreFloat3(&projRightPos, XMVectorAdd(XMLoadFloat3(&projRightPos), m_pCamera->GetLook()));
	projRightPos.x += m_ShootOffset;

	ShootProjectile(projRightPos, m_pCamera->GetLook3f());
}

void PlayerShoot::ShootProjectile(XMFLOAT3 position, XMFLOAT3 direction)
{
	const auto projectile = GameObject::Instantiate<Projectile>()->GetComponent<ProjectileBehavior>();
	projectile->transform->SetPosition(position);
	projectile->Initialize(direction, m_ProjectileSpeed, m_ProjectileLifeTime);
}


void PlayerShoot::HandleZoomAndSlowMotion()
{
	if (InputHandler::IsKeyHeld(VK_RBUTTON))
	{
		//m_pCamera->SetFOV(m_BasicFOV / m_Zoom);
		float currentTimeScale = TimeManager::GetTimeScale();

		if (currentTimeScale > m_SlowMotion)
		{
			currentTimeScale -= m_SlowMotionSpeed * TimeManager::GetUnscaledDeltaTime();
			TimeManager::SetTimeScale(std::clamp(currentTimeScale, 0.1f, 1.f));
		}
	}
	else if (TimeManager::GetTimeScale() < 1)
	{
		//m_pCamera->SetFOV(m_BasicFOV);
		TimeManager::SetTimeScale(std::clamp(TimeManager::GetTimeScale() + m_SlowMotionRecoverSpeed * TimeManager::GetUnscaledDeltaTime(), 0.1f, 1.f));
	}

	DEBUG_LOG("TimeScale: " + std::to_string(TimeManager::GetTimeScale()));
}