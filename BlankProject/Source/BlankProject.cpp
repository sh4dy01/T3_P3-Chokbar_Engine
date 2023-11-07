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
	auto * test3 = GameObject::Instantiate<Asteroid>();
	test3->transform->SetPosition(0, 0, 0);


	auto player = GameObject::Instantiate<Player>();
	player->transform->SetPosition(0, 0, -5);

	GameObject::Instantiate<SkyBox>();

	auto * test = GameObject::Instantiate<Asteroid>();
	test->transform->SetPosition(0, 0, 5);
	//test->transform->SetParent(player->transform);

	test3 = nullptr;
	player = nullptr;
}

void Application::Update(const float dt)

{
}

void Application::Shutdown()
{

}