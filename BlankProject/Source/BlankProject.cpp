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
	//player->GetComponent<Rigidbody>()->Move(0.f, 0.f, -50.f);

	player->GetComponent<Rigidbody>()->Move(3.f, 3.f, 9.f);
	player->transform->RotateYaw(180);


	/*player->transform->SetPosition(0, 0, 50);
	player->transform->RotateYaw(180);*/

	player->m_CategoryBitmask.SetLayer(LayerID::PLAYER);
	player->m_CollisionBitmask.SetLayer(LayerID::ASTEROID);

	for (int i = 0; i < 0; i++)
	{
		auto asteroid = GameObject::Instantiate<Asteroid>();
		asteroid->m_CategoryBitmask.SetLayer(LayerID::ASTEROID);
		asteroid->m_CollisionBitmask.SetLayer(LayerID::PLAYER);
		//asteroid->m_CollisionBitmask.AddLayer(LayerID::ASTEROID);
		asteroid->m_CollisionBitmask.AddLayer(LayerID::PROJECTILE);


		float x = (rand() % 50) - 5.5f;
		float y = (rand() % 50) - 5.5f;
		float z = (rand() % 50) - 5.5f;
		asteroid->GetComponent<Rigidbody>()->Move(3, 3, 3);


		/*auto go = GameObject::Instantiate<GameObject>();
		auto mr = new MeshRenderer();
		mr->Init(SPHERE, SIMPLE);
		go->AddComponent<MeshRenderer>(mr);
		go->transform->SetPosition(x, y, z);*/
		//go->transform->
	}

	GameObject::Instantiate<SkyBox>();
  
	auto* test = GameObject::Instantiate();
	test->transform->SetPosition(-3, 0, 25);
	test->transform->SetScale(3.f, 3.f, 3.f);

	test->AddComponent<MeshRenderer>();
	auto pr = test->AddComponent<ParticleRenderer>();
	pr->Init(MeshType::CUBE, MaterialType::PARTICLE);
	pr->SetParticleCount(100);
	pr->Play();
  
	player = nullptr;
}

void Application::Update(const float dt)

{
}

void Application::Shutdown()
{

}