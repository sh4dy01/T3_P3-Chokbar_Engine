#include "BlankProject.h"

#include "Engine/Resource.h"

#include "Platform/Windows/WinEntry.h"

using namespace Chokbar;

class Application : public Win32::IApplication {

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
		
	auto* test2 = new GameObject("ball2");
	auto* mr2 = new MeshRenderer(MeshType::SPHERE, MaterialType::TEXTURE);
	test2->AddComponent<MeshRenderer>(mr2);
	std::string path2 = "Resources/Textures/mars.dds";
	test2->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path2));
	test2->transform->SetPosition(-1, 1, 1);

	auto* test = new GameObject("ball1");
	auto* mr = new MeshRenderer(MeshType::SPHERE, MaterialType::TEXTURE);
	test->AddComponent<MeshRenderer>(mr);
	std::string path = "Resources/Textures/4k.dds";
	test->GetComponent<MeshRenderer>()->RegisterTexture(Resource::Load<Texture>(path));
	test->transform->SetPosition(0, 0, 2);
	

	/*auto* test2 = new GameObject("f");
	auto* mr2 = new MeshRenderer(MeshType::PYRAMID, MaterialType::SIMPLE);
	test2->AddComponent<MeshRenderer>(mr2);
	test2->transform->SetPosition(0, 0, -2);*/


	auto test3 = GameObject::Instantiate("pyr");
	test3->AddComponent<MeshRenderer>(new MeshRenderer(MeshType::PYRAMID, MaterialType::SIMPLE));
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