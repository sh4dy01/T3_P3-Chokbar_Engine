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
	auto* test = new GameObject("ball");
	test->transform->SetPosition(-3, 0, 10);


	auto* mr = new MeshRenderer();
	auto* pr = new ParticleRenderer();

	test->AddComponent<MeshRenderer>(mr);
	//test->AddComponent<ParticleRenderer>(pr);
	mr->Init(MeshType::SPHERE, MaterialType::TEXTURE);
	//pr->Init(MeshType::CUBE, MaterialType::PARTICLE);
	//pr->SetParticleCount(100);

	std::string path = "Resources/Textures/mars.dds";
	test->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path));

	auto* test2 = new GameObject("sky");
	test2->transform->SetPosition(0, 0, 0);
	test2->transform->SetScale(5000, 5000, 5000);

	auto* mr2 = new MeshRenderer();
	mr2->Init(MeshType::CUBE, MaterialType::SKYBOX);
	std::string path2 = "Resources/Textures/spaceCM.dds";
	mr2->RegisterTexture(Resource::Load<Texture>(path2));
	test2->AddComponent<MeshRenderer>(mr2);

	auto* text1 = new GameObject("text1");
	auto* mrt1 = new MeshRenderer();
	text1->AddComponent<MeshRenderer>(mrt1);
	mrt1->Init(MeshType::CUBE, MaterialType::TEXTURE_TRANSPARENT);
	std::string path3 = "Resources/Textures/number.dds";
	mrt1->RegisterTexture(Resource::Load<Texture>(path3));
	text1->transform->SetPosition(0, 0, 0);

	auto* text2 = new GameObject("text2");
	auto* mrt2 = new MeshRenderer();
	text2->AddComponent<MeshRenderer>(mrt2);
	mrt2->Init(MeshType::CUBE, MaterialType::TEXTURE_TRANSPARENT);
	std::string path4 = "Resources/Textures/number.dds";
	mrt2->RegisterTexture(Resource::Load<Texture>(path4));
	text2->transform->SetPosition(1, 0, 0);

	auto* text3 = new GameObject("text3");
	auto* mrt3 = new MeshRenderer();
	text3->AddComponent<MeshRenderer>(mrt3);
	mrt3->Init(MeshType::CUBE, MaterialType::TEXTURE_TRANSPARENT);
	std::string path5 = "Resources/Textures/number.dds";
	mrt3->RegisterTexture(Resource::Load<Texture>(path5));
	text3->transform->SetPosition(2, 0, 0);

	auto player = GameObject::Instantiate<Player>();
	player->transform->SetPosition(0, 0, -5);

	player = nullptr;

	//auto skybox = GameObject::Instantiate<SkyBox>();

	//skybox = nullptr;
}

void Application::Run()
{
	Engine::GetInstance()->Run();
}

void Application::Update(const float dt)
{
	GameObject* text1 = GameObject::Find("text1");
}

void Application::Shutdown()
{
	Engine::GetInstance()->Shutdown();
}