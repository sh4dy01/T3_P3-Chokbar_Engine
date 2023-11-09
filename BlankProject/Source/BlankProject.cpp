#include "BlankProject.h"
#include "Platform/Windows/WinEntry.h"

#include "GameObjects/Camera.h"
#include "GameObjects/Player.h"
#include "GameObjects/Asteroid.h"
#include "GameObjects/Skybox.h"
#include "GameObjects/UI/Score.h"
#include "GameObjects/UI/CrossAir.h"
#include "GameObjects/Particles/ProjectileParticles.h"
#include "GameObjects/Planet.h"


class Application : public Win32::IApplication
{

public:
	Application() {}
	~Application() {}

public:
	void SetupPerGameSettings() override;

	/* Initialize the application */
	void Initialize() override;

	/* Game Loop */
	void Update(const float dt) override;

	void Shutdown() override;
};

ENTRYAPP(Application);

void Application::SetupPerGameSettings()
{
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
}


void Application::Initialize()
{
	GameObject::Instantiate<ProjectileParticles>();

	
	auto camera = GameObject::Instantiate<Camera>();

	const auto player = GameObject::Instantiate<Player>();
	player->GetComponent<Rigidbody>()->Move(0, 0, 75);
	player->transform->RotateYaw(180);

	camera->transform->SetParent(player->transform);

	for (int i = 0; i < 100; i++)
	{
		const auto asteroid = GameObject::Instantiate<Planet>();
		
		const float x = (rand() % 10) - 10;
		const float y = (rand() % 10) - 10;
		const float z = (rand() % 10) - 10;

		asteroid->GetComponent<Rigidbody>()->Move(x, y, z);
		
	}

	GameObject::Instantiate<SkyBox>();

	GameObject::Instantiate<Score>();

	auto crossAir = GameObject::Instantiate<CrossAir>();
	crossAir->transform->SetPosition(0, 0, 0);

	camera = nullptr;
	crossAir = nullptr;
}

void Application::Update(const float dt)
{

}

void Application::Shutdown()
{

}