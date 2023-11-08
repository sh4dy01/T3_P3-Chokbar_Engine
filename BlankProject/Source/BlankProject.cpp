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
	auto camera = GameObject::Instantiate<Camera>();

	auto player = GameObject::Instantiate<Player>();
	player->transform->SetPosition(0, 0, -5);
	camera->transform->SetParent(player->transform);

	for (int i = 0; i < 30; i++)
	{
		auto asteroid = GameObject::Instantiate<Asteroid>();

		float x = (rand() % 100) - 5.5f;
		float y = (rand() % 100) - 5.5f;
		float z = (rand() % 100) - 5.5f;

		asteroid->GetComponent<Rigidbody>()->Move(x, y, z);
	}

	GameObject::Instantiate<SkyBox>();
  
	auto* test = GameObject::Instantiate();
	test->transform->SetPosition(-3, 0, 25);
	test->transform->SetScale(3.f, 3.f, 3.f);


	auto mr = test->AddComponent<MeshRenderer>();
	auto pr = test->AddComponent<ParticleRenderer>();
	mr->Init(MeshType::PYRAMID, MaterialType::TEXTURE);
	mr->RegisterTexture(Resource::Load<Texture>("Resources/Textures/mars.dds"));
	pr->Init(MeshType::CUBE, MaterialType::PARTICLE);
	pr->SetParticleCount(100);
	pr->Play();


	test = nullptr;
	mr = nullptr;
	pr = nullptr;
	player = nullptr;
}

void Application::Update(const float dt)

{
}

void Application::Shutdown()
{

}