#include "BlankProject.h"
#include "Platform/Windows/WinEntry.h"

#include "Engine/Resource.h"
#include "Engine/Engine.h"

#include "GameObjects/Camera.h"
#include "GameObjects/Player.h"
#include "GameObjects/Asteroid.h"



class Application : public Win32::IApplication
{

public:
	Application() {}
	~Application() {}

public:
	void SetupPerGameSettings() override;

	/* Initialize the application */
	void Initialize() override;

	void PreInitialize() override;

	/* Game Loop */
	void Update(const float dt) override;

	void Run() override;

	void Shutdown() override;
};

ENTRYAPP(Application);

void Application::SetupPerGameSettings()
{
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
}

void Application::PreInitialize()
{
	Engine::GetInstance()->Initialize();
}

void Application::Initialize()
{
	GameObject::Instantiate<Player>()->transform->SetPosition(0, 0, -5);

	DirectX::XMFLOAT3 pos = { 0, 0, 0 };
	float scale = 1;

	int range = 10;
	int scaleRange = 10;
	for (int i = 0; i < 100; i++)
	{
		// create a vector3 with random values of range -10 to 10
		pos = {
			static_cast<float>(rand() % range - range/2),
			static_cast<float>(rand() % range - range / 2),
			0
		};

		scale  = static_cast<float>(rand() % scaleRange + 1);

		const auto go = GameObject::Instantiate<Asteroid>();
		go->transform->SetPosition(pos);
		//go->transform->SetScale(scale);
	}
}

void Application::Run()
{
	Engine::GetInstance()->Run();
}

void Application::Update(const float dt)

{
}

void Application::Shutdown()
{
	Engine::GetInstance()->Shutdown();
}