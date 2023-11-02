#include "BlankProject.h"

#include "Platform/Windows/WinEntry.h"
#include "Engine/ECS/Base/GameObject.h"
#include "Core/D3D/Internal/MeshRenderer.h"
#include "Engine/ECS/Components/PlayerComponent.h"
#include "Engine/ECS/Components/Collision/Rigidbody.h"
#include "Engine/ECS/Components/Collision/SphereCollider.h"

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
	auto *test = new GameObject("Pyr1");
	auto *mr = new MeshRenderer(MeshType::PYRAMID, MaterialType::SIMPLE);
	auto *rb = new Rigidbody(false);
	auto *sphere = new SphereCollider(XMFLOAT3(0,0,0), 0);
	test->AddComponent<MeshRenderer>(mr);
	test->AddComponent<Rigidbody>(rb);
	test->AddComponent<SphereCollider>(sphere);
	test->transform->SetPosition(0, 0, -16);

	auto *test2 = new GameObject("Pyr2");
	auto *mr2 = new MeshRenderer(MeshType::PYRAMID, MaterialType::SIMPLE);
	auto *rb2 = new Rigidbody(false);
	auto *sphere2 = new SphereCollider(XMFLOAT3(0, 0, 0),0);
	test2->AddComponent<MeshRenderer>(mr2);
	test2->AddComponent<Rigidbody>(rb2);
	test2->AddComponent<SphereCollider>(sphere2);
	test2->transform->SetPosition(0, 0, -16);

	auto *test3 = new GameObject("Pyr3");
	auto *mr3 = new MeshRenderer(MeshType::PYRAMID, MaterialType::SIMPLE);
	auto *rb3 = new Rigidbody(false);
	auto *sphere3 = new SphereCollider(XMFLOAT3(0, 0, 0), 0);
	test3->AddComponent<MeshRenderer>(mr3);
	test3->AddComponent<Rigidbody>(rb3);
	test3->AddComponent<SphereCollider>(sphere3);
	test2->transform->SetPosition(-2, 1, -2);
	test3->transform->SetScale(6, 6, 6);

	// GameObject player = GameObject("player");
	// player.AddComponent<PlayerComponent>();
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