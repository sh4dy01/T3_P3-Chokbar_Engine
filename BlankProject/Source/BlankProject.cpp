#include "BlankProject.h"

#include "Engine/Resource.h"

#include "Platform/Windows/WinEntry.h"
#include "Engine/ECS/Base/GameObject.h"
#include "Core/D3D/Internal/MeshRenderer.h"
#include "Engine/ECS/Components/PlayerComponent.h"
#include "Engine/ECS/Components/Collision/Rigidbody.h"
#include "Engine/ECS/Components/Collision/SphereCollision.h"

using namespace Chokbar;

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

	auto* test = new GameObject("f");
	auto* mr = new MeshRenderer(MeshType::SPHERE, MaterialType::TEXTURE);
	test->AddComponent<MeshRenderer>(mr);
	std::string path = "Resources/Textures/mars.dds";
	test->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path));
	test->transform->SetPosition(0, 0, 2);

	/*auto* test2 = new GameObject("f");
	auto* mr2 = new MeshRenderer(MeshType::PYRAMID, MaterialType::SIMPLE);
	test2->AddComponent<MeshRenderer>(mr2);
	test2->transform->SetPosition(0, 0, -2);*/
	//test2->transform->SetPosition(-2, 1, -2);

	auto *test3 = new GameObject("f");
	auto *mr3 = new MeshRenderer(MeshType::PYRAMID, MaterialType::SIMPLE);
	auto *rb3 = new Rigidbody();
	auto *sphere3 = new Sphere(XMFLOAT3(0, 0, 0), 2.0f);
	test3->AddComponent<MeshRenderer>(mr3);
	test3->AddComponent<Rigidbody>(rb3);
	test3->AddComponent<Sphere>(sphere3);
	test3->transform->SetScale(6, 6, 6);

	//GameObject player = GameObject("player");
	//player.AddComponent<PlayerComponent>();
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