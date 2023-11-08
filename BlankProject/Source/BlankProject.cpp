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

	for (int i = 0; i < 30; i++)
	{
		auto asteroid = GameObject::Instantiate<Asteroid>();
		asteroid->m_CategoryBitmask.SetLayer(LayerID::ASTEROID);
		asteroid->m_CollisionBitmask.SetLayer(LayerID::PLAYER);
		//asteroid->m_CollisionBitmask.AddLayer(LayerID::ASTEROID);
		asteroid->m_CollisionBitmask.AddLayer(LayerID::PROJECTILE);


		float x = (rand() % 100) - 5.5f;
		float y = (rand() % 100) - 5.5f;
		float z = (rand() % 100) - 5.5f;
		asteroid->GetComponent<Rigidbody>()->Move(x, y, z);
	}

	GameObject::Instantiate<SkyBox>();
  
	auto* test = NEW GameObject("ball");
	test->transform->SetPosition(-3, 0, 25);
	test->transform->SetScale(3.f, 3.f, 3.f);
	auto camera = GameObject::Instantiate<Camera>();

	auto* mr = NEW MeshRenderer();
	auto* pr = NEW ParticleRenderer();

	test->AddComponent<MeshRenderer>(mr);
	test->AddComponent<ParticleRenderer>(pr);
	mr->Init(MeshType::PYRAMID, MaterialType::TEXTURE);
	pr->Init(MeshType::CUBE, MaterialType::PARTICLE);
	pr->SetParticleCount(100);
	pr->Play();

	test->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>("Resources/Textures/mars.dds"));

	auto * test3 = NEW GameObject("asteroid");
	test3->transform->SetPosition(3, 0, 7);

	auto* mr3 = new MeshRenderer();
	mr3->Init(MeshType::SPHERE, MaterialType::TEXTURE);
	mr3->RegisterTexture(Resource::Load<Texture>("Resources/Textures/4k.dds"));
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

void Application::Update(const float dt)

{
}

void Application::Shutdown()
{

}