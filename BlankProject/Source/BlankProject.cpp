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
	auto* player = GameObject::Instantiate<Player>("Player");
	player->transform->Translate(XMFLOAT3(0, 0, -5));

	auto* ship = new GameObject("SHIP");

	auto* meshRenderer = new MeshRenderer();
	meshRenderer->Init(MeshType::SPHERE, MaterialType::TEXTURE);

	auto* collider = new SphereCollider();

	auto* rigidbody = new Rigidbody();

	ship->AddComponent<MeshRenderer>(meshRenderer);
	ship->AddComponent<Rigidbody>(rigidbody);
	ship->AddComponent<SphereCollider>(collider);
	std::string path = "Resources/Textures/mars.dds";
	ship->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path));
	ship->GetComponent<Rigidbody>()->Move(XMFLOAT3(0, 0, 2));

	auto* asteroid = GameObject::Instantiate<Asteroid>("Asteroid");
	asteroid->GetComponent<Rigidbody>()->Move(1, 0, 2);

	//for (int i = 0; i < 10; i++)
	//{
	//	auto* asteroid = GameObject::Instantiate<Asteroid>("Asteroid");
	//	asteroid->GetComponent<Rigidbody>()->Move(XMFLOAT3(rand() % 3 - 3/2, rand() % 3 - 3/2, rand() % 3 - 3 / 2));
	//	//asteroid->GetComponent<Rigidbody>()->Move(10 * i, 0, 0);

	//	/*auto* asteroid = new GameObject("ASTEROID");

	//	auto* meshRendererAsteroid = new MeshRenderer();
	//	meshRendererAsteroid->Init(MeshType::SPHERE, MaterialType::TEXTURE);

	//	auto* colliderAsteroid = new SphereCollider();

	//	auto* rigidbodyAsteroid = new Rigidbody();

	//	std::string pathAsteroid = "Resources/Textures/4k.dds";
	//	asteroid->AddComponent<MeshRenderer>(meshRendererAsteroid);
	//	asteroid->AddComponent<Rigidbody>(rigidbodyAsteroid);
	//	asteroid->AddComponent<SphereCollider>(colliderAsteroid);
	//	asteroid->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path));
	//	asteroid->GetComponent<Rigidbody>()->Move(XMFLOAT3(rand() % 3 - 3 / 2, rand() % 3 - 3 / 2, 2));*/
	//}


	/*auto* asteroid = new GameObject("ASTEROID");

	auto* meshRendererAsteroid = new MeshRenderer();
	meshRendererAsteroid->Init(MeshType::SPHERE, MaterialType::TEXTURE);

	auto* colliderAsteroid = new SphereCollider();

	auto* rigidbodyAsteroid = new Rigidbody();

	std::string pathAsteroid = "Resources/Textures/4k.dds";
	asteroid->AddComponent<MeshRenderer>(meshRendererAsteroid);
	asteroid->AddComponent<Rigidbody>(rigidbodyAsteroid);
	asteroid->AddComponent<SphereCollider>(colliderAsteroid);
	asteroid->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path));
	asteroid->GetComponent<Rigidbody>()->Move(XMFLOAT3(0, 2, 2));*/

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