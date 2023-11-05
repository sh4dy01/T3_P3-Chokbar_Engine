#include "BlankProject.h"
#include "PlayerShoot.h"

#include "ProjectileBehavior.h"
#include "GameObjects/Projectile.h"


void PlayerShoot::Awake()
{
	m_pCamera = gameObject->GetComponent<CameraComponent>();
}

void PlayerShoot::Start()
{
	m_pCamera->SetFOV(m_BasicFOV);
}

void PlayerShoot::Update()
{
	if (InputHandler::IsKeyDown(VK_LBUTTON))
	{
		auto proj = GameObject::Instantiate<Projectile>()->GetComponent<ProjectileBehavior>();

		XMFLOAT3 spawn;
		spawn.x = transform->GetPosition().x;
		spawn.y = transform->GetPosition().y;
		spawn.z = transform->GetPosition().z + 5;
		proj->transform->SetPosition(spawn);
		proj->Initialize(transform->GetForward(), 3, 2);
	}

	HandleZoomAndSlowMotion();
}


void PlayerShoot::HandleZoomAndSlowMotion()
{
	if (InputHandler::IsKeyDown(VK_RBUTTON))
	{
		//m_pCamera->SetFOV(m_BasicFOV / m_Zoom);
		TimeManager::SetTimeScale(m_SlowMotion);
	}
	else if (InputHandler::IsKeyUp(VK_RBUTTON))
	{
		//m_pCamera->SetFOV(m_BasicFOV);
		TimeManager::SetTimeScale(1.f);
	}
}