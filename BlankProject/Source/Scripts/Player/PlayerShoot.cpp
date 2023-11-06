#include "BlankProject.h"
#include "PlayerShoot.h"

#include "ProjectileBehavior.h"
#include "GameObjects/Projectile.h"


void PlayerShoot::Awake()
{
	m_pCamera = gameObject->GetComponent<CameraComponent>();
	m_pRigidbody = gameObject->GetComponent<Rigidbody>();
}

void PlayerShoot::Start()
{
	m_pCamera->SetFOV(m_BasicFOV);
}

void PlayerShoot::Update()
{
	if (InputHandler::IsKeyDown(VK_LBUTTON))
	{
		const auto proj = GameObject::Instantiate<Projectile>()->GetComponent<ProjectileBehavior>();

		XMFLOAT3 projPos = transform->GetPosition();
		XMStoreFloat3(&projPos, XMVectorAdd(XMLoadFloat3(&projPos), m_pCamera->GetLook()));
		proj->transform->SetPosition(projPos);
		proj->Initialize(m_pCamera->GetLook3f(), 3, 2);
	}

	HandleZoomAndSlowMotion();
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