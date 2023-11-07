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
	auto* test = NEW GameObject("ball");
	test->transform->SetPosition(-3, 0, 10);
	test->transform->SetScale(3.f, 3.f, 3.f);

	auto* mr = NEW MeshRenderer();
	auto* pr = NEW ParticleRenderer();

	test->AddComponent<MeshRenderer>(mr);
	test->AddComponent<ParticleRenderer>(pr);
	mr->Init(MeshType::SPHERE, MaterialType::TEXTURE);
	pr->Init(MeshType::CUBE, MaterialType::PARTICLE);
	pr->SetParticleCount(100);
	pr->Play();

	test->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>("Resources/Textures/mars.dds"));

	/*
	auto * test3 = NEW GameObject("asteroid");
	test3->transform->SetPosition(3, 0, 7);

	auto* mr3 = new MeshRenderer();
	mr3->Init(MeshType::SPHERE, MaterialType::TEXTURE);
	mr3->RegisterTexture(Resource::Load<Texture>("Resources/Textures/mars.dds"));
	test3->AddComponent<MeshRenderer>(mr3);
	*/

	auto player = GameObject::Instantiate<Player>();
	player->transform->SetPosition(0, 0, -5);

	GameObject::Instantiate<SkyBox>();

	test = nullptr;
	mr = nullptr;
	pr = nullptr;
	//test3 = nullptr;
	//mr3 = nullptr;
	player = nullptr;
}

void Application::Update(const float dt)

{
}

void Application::Shutdown()
{

}