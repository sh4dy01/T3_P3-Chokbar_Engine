#include "BlankProject.h"
#include "Platform/Windows/WinEntry.h"

#include "GameObjects/Camera.h"
#include "GameObjects/Player.h"
#include "GameObjects/Asteroid.h"
#include "GameObjects/Skybox.h"
#include "GameObjects/UI/Score.h"
#include "GameObjects/Particles/ProjectileParticles.h"


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

	auto player = GameObject::Instantiate<Player>();
	player->GetComponent<Rigidbody>()->Move(3.f, 3.f, 9.f);
	player->transform->SetPosition(0, 0, -5);

	camera->transform->SetParent(player->transform);

	for (int i = 0; i < 50; i++)
	{
		const auto asteroid = GameObject::Instantiate<Asteroid>();
		
		const float x = (rand() % 50) - 5.5f;
		const float y = (rand() % 50) - 5.5f;
		const float z = (rand() % 50) - 5.5f;
		asteroid->GetComponent<Rigidbody>()->Move(x, y, z);
	}

	GameObject::Instantiate<SkyBox>();

	GameObject::Instantiate<Score>();

	player = nullptr;
	camera = nullptr;
}

void Application::Update(const float dt)
{

}

void Application::Shutdown()
{

}