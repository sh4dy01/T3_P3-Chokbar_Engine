#include "BlankProject.h"

#include "Engine/Engine.h"
#include "Platform/Windows/WinEntry.h"


class Application : public Win32::IApplication {

public:

	Application() {}
	~Application() {}

public:

	void SetupPerGameSettings() override;

	/* Initialize the application */
	void Initialize() override;

	void PreInitialize() override {};

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

// Init engine
void Application::Initialize()
{

}

// Main loop
void Application::Run()
{
	Chokbar::Engine& engine = Chokbar::Engine::GetInstance();

	engine.Initialize();

	// Main loop
	engine.Run();
	engine.Shutdown();
}

void Application::Update(const float dt)
{
	
}

void Application::Shutdown()
{

}