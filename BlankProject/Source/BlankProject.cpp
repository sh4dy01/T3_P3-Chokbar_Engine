#include "BlankProject.h"

#include "Engine/Engine.h"
#include "Platform/Windows/WinEntry.h"
#include "Engine/ECS/Base/GameObject.h"
#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/ECS/Components/PlayerComponent.h"
#include "Engine/ECS/Components/Collision/Rigidbody.h"

using namespace Chokbar;

class Application : public Win32::IApplication {

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
	Engine::GetInstance().Initialize();
}

void Application::Initialize()
{
	auto test = new GameObject("player");
	auto player = new GameObject("test");
	auto rbA = new RigidBody();
	auto rbB = new RigidBody();
	
	test->AddComponent<RigidBody>(rbA);
	player->AddComponent<RigidBody>(rbB);

	auto sphereA = new Sphere(XMFLOAT3(0, 0, 0), 2);
	auto sphereB = new Sphere(XMFLOAT3(0, 0, 0), 2);
	test->AddComponent<Sphere>(sphereA);
	player->AddComponent<Sphere>(sphereB);
}

void Application::Run()
{
	Engine::GetInstance().Run();
}

void Application::Update(const float dt)

{
	
}

void Application::Shutdown()
{
	Engine::GetInstance().Shutdown();
}