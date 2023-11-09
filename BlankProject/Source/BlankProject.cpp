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
	player->m_CategoryBitmask.SetLayer(LayerID::PLAYER);
	player->m_CollisionBitmask.SetLayer(LayerID::ASTEROID);
	player->GetComponent<Rigidbody>()->Move(0.f, 0.f, -10.f);

	auto asteroid = GameObject::Instantiate<Asteroid>();
	asteroid->m_CategoryBitmask.SetLayer(LayerID::ASTEROID);
	asteroid->m_CollisionBitmask.SetLayer(LayerID::PLAYER);
	asteroid->m_CollisionBitmask.AddLayer(LayerID::PROJECTILE);
	asteroid->m_CollisionBitmask.AddLayer(LayerID::ASTEROID);
	asteroid->GetComponent<Rigidbody>()->Move(-2, 0, 0);
	asteroid->GetComponent<Rigidbody>()->SetVelocity({ 1.0f, 0.1f, 0 });

	auto asteroid2 = GameObject::Instantiate<Asteroid>();
	asteroid2->m_CategoryBitmask.SetLayer(LayerID::ASTEROID);
	asteroid2->m_CollisionBitmask.SetLayer(LayerID::PLAYER);
	asteroid2->m_CollisionBitmask.AddLayer(LayerID::PROJECTILE);
	asteroid2->m_CollisionBitmask.AddLayer(LayerID::ASTEROID);
	asteroid2->GetComponent<Rigidbody>()->Move(2, 0, 0);
	asteroid2->GetComponent<Rigidbody>()->SetVelocity({ -1.0f, -0.1f, 0 });

	/*auto collider = GameObject::Instantiate<GameObject>();
	collider->AddComponent<MeshRenderer>()->Init(SPHERE, SIMPLE);
	collider->transform->SetScale(asteroid2->GetComponent<SphereCollider>()->GetRadius());
	XMFLOAT3 spherPos = asteroid2->transform->GetPosition();
	spherPos.x += asteroid2->GetComponent<SphereCollider>()->GetCenter().x;
	spherPos.y += asteroid2->GetComponent<SphereCollider>()->GetCenter().y;
	spherPos.z += asteroid2->GetComponent<SphereCollider>()->GetCenter().z;

	collider->transform->SetPosition(spherPos);*/

	//for (int i = 0; i < 1; i++)
	//{
	//	auto asteroid = GameObject::Instantiate<Asteroid>();
	//	asteroid->m_CategoryBitmask.SetLayer(LayerID::ASTEROID);
	//	asteroid->m_CollisionBitmask.SetLayer(LayerID::PLAYER);
	//	//asteroid->m_CollisionBitmask.AddLayer(LayerID::ASTEROID);
	//	asteroid->m_CollisionBitmask.AddLayer(LayerID::PROJECTILE);


	//	float x = (rand() % 50) - 5.5f;
	//	float y = (rand() % 50) - 5.5f;
	//	float z = (rand() % 50) - 5.5f;
	//	asteroid->GetComponent<Rigidbody>()->Move(3, 3, 3);
	//	asteroid->GetComponent<Rigidbody>()->SetVelocity({ 0, 0, 0.1f });

	//	/*auto go = GameObject::Instantiate<GameObject>();
	//	auto mr = new MeshRenderer();
	//	mr->Init(SPHERE, SIMPLE);
	//	go->AddComponent<MeshRenderer>(mr);
	//	go->transform->SetPosition(x, y, z);*/
	//	//go->transform->
	//}

	GameObject::Instantiate<SkyBox>();

	auto* test = NEW GameObject("ball");
	test->transform->SetPosition(-3, 0, 25);
	test->transform->SetScale(3.f, 3.f, 3.f);

	auto* mr = NEW MeshRenderer();
	test->AddComponent<MeshRenderer>(mr);
	auto* pr = NEW ParticleRenderer();
	test->AddComponent<ParticleRenderer>(pr);
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