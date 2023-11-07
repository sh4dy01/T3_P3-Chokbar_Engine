#include "BlankProject.h"
#include "Platform/Windows/WinEntry.h"

#include "GameObjects/Camera.h"
#include "GameObjects/Player.h"
#include "GameObjects/Asteroid.h"
#include "GameObjects/Skybox.h"



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
	auto player = GameObject::Instantiate<Player>();
	player->transform->SetPosition(0, 0, -50);

	for (int i = 0; i < 300; i++)
	{
		auto asteroid = GameObject::Instantiate<Asteroid>();
		float x = (rand() % 3) - 1.5f;
		float y = (rand() % 3) - 1.5f;
		float z = (rand() % 3) - 1.5f;
		asteroid->transform->SetPosition(x, y, z);
	}

	GameObject::Instantiate<SkyBox>();

	player = nullptr;
}

void Application::Update(const float dt)

{
}

void Application::Shutdown()
{

}