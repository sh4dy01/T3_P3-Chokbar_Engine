#include "BlankProject.h"

#include "Engine/Engine.h"
#include "Platform/Windows/WinEntry.h"


class Application : public Chokbar::Engine {

public:

	Application() {}
	~Application() {}

public:

	void SetupPerGameSettings();

	/* Initialize the application */
	void Initialize() override;

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

void Application::Initialize()
{
	Engine::Initialize();
}


void Application::Run()
{
	Engine::ResetTimer();

	while (!Engine::NeedsToClose())
	{
		Window::PollEvent();

		Engine::Tick();

		Update(GetDeltaTime());
		Engine::Render();
	}
}

void Application::Update(const float dt)
{
	Engine::Update(dt);
}

void Application::Shutdown()
{
	Engine::Shutdown();
}
