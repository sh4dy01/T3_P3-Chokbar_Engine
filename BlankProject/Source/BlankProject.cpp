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
	GameObject::Instantiate<Player>()->transform->SetPosition(0, 0, -5);
	auto origin = GameObject::Instantiate()->AddComponent<MeshRenderer>(new MeshRenderer(PYRAMID, SIMPLE));

	DirectX::XMFLOAT3 pos = { 0, 0, 0 };
	float scale = 1;

	auto go = GameObject::Instantiate<Asteroid>();
	go->transform->SetPosition(pos);
	go->transform->SetScale(scale);
	go->GetComponent<MeshRenderer>()->Bounds.Center = pos;
	go->GetComponent<MeshRenderer>()->Bounds.Radius = scale;


	scale = 2;
	pos.x = 3;
	go = GameObject::Instantiate<Asteroid>();
	go->transform->SetPosition(pos);
	go->transform->SetScale(scale);
	go->GetComponent<MeshRenderer>()->Bounds.Center = pos;
	go->GetComponent<MeshRenderer>()->Bounds.Radius = scale;



	scale = 3;
	pos.x = 8;

	go = GameObject::Instantiate<Asteroid>();
	go->transform->SetPosition(pos);
	go->transform->SetScale(scale);
	go->GetComponent<MeshRenderer>()->Bounds.Center = pos;
	go->GetComponent<MeshRenderer>()->Bounds.Radius = scale;



	/*
	int range = 100;
	int scaleRange = 5;
	for (int i = 0; i < 200; i++)
	{
		// create a vector3 with random values of range -10 to 10
		pos = {
			static_cast<float>(rand() % range - range/2),
			static_cast<float>(rand() % range - range / 2),
			0
		};

		scale  = static_cast<float>(rand() % scaleRange + 1);

		auto go = GameObject::Instantiate<Asteroid>();
		go->transform->SetPosition(pos);
		go->transform->SetScale(scale);

		go->GetComponent<MeshRenderer>()->Bounds.Center = pos;
		go->GetComponent<MeshRenderer>()->Bounds.Radius = scale;

	player = nullptr;
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