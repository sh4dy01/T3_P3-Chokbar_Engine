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
	player->m_CategoryBitmask.SetLayer(LayerID::PLAYER);
	player->m_CollisionBitmask.SetLayer(LayerID::ASTEROID);

	for (int i = 0; i < 150; i++)
	{
		auto asteroid = GameObject::Instantiate<Asteroid>();
		asteroid->m_CategoryBitmask.SetLayer(LayerID::ASTEROID);
		asteroid->m_CollisionBitmask.SetLayer(LayerID::PLAYER);
		//asteroid->m_CollisionBitmask.AddLayer(LayerID::ASTEROID);
		asteroid->m_CollisionBitmask.AddLayer(LayerID::PROJECTILE);


		float x = (rand() % 50) - 5.5f;
		float y = (rand() % 50) - 5.5f;
		float z = (rand() % 50) - 5.5f;
		asteroid->GetComponent<Rigidbody>()->Move(x, y, z);
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