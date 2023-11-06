#include "BlankProject.h"
#include "Platform/Windows/WinEntry.h"

#include "Engine/Engine.h"

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
	auto* test = NEW GameObject("ball");
	test->transform->SetPosition(-3, 0, 10);

	auto* mr = NEW MeshRenderer();
	auto* pr = NEW ParticleRenderer();

	test->AddComponent<MeshRenderer>(mr);
	test->AddComponent<ParticleRenderer>(pr);
	mr->Init(MeshType::PYRAMID, MaterialType::TEXTURE);
	pr->Init(MeshType::CUBE, MaterialType::PARTICLE);
	pr->SetParticleCount(100);

	std::string path = "Resources/Textures/mars.dds";
	test->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path));

	auto * test3 = NEW GameObject("asteroid");
	test3->transform->SetPosition(3, 0, 7);

	auto* mr3 = NEW MeshRenderer();
	mr3->Init(MeshType::CUBE, MaterialType::TEXTURE);
	std::string path3 = "Resources/Textures/angry_winnie.dds";
	mr3->RegisterTexture(Resource::Load<Texture>(path3));
	test3->AddComponent<MeshRenderer>(mr3);


	auto player = GameObject::Instantiate<Player>();
	player->transform->SetPosition(0, 0, -5);

	GameObject::Instantiate<SkyBox>();

	test = nullptr;
	mr = nullptr;
	pr = nullptr;
	test3 = nullptr;
	mr3 = nullptr;
	player = nullptr;
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