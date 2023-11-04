#include "BlankProject.h"
#include "Platform/Windows/WinEntry.h"

#include "Engine/Resource.h"
#include "Core/D3D/Internal/ParticleRenderer.h"
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
	auto* test = new GameObject("ball");
	test->transform->SetPosition(-3, 0, 10);


	auto* mr = new MeshRenderer();
	auto* pr = new ParticleRenderer();

	test->AddComponent<MeshRenderer>(mr);
	test->AddComponent<ParticleRenderer>(pr);
	mr->Init(MeshType::SPHERE, MaterialType::TEXTURE);
	pr->Init(MeshType::CUBE, MaterialType::PARTICLE);
	pr->SetParticleCount(100);

	std::string path = "Resources/Textures/mars.dds";
	test->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path));


	auto player = GameObject::Instantiate<Player>();
	player->transform->SetPosition(0, 0, -5);

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