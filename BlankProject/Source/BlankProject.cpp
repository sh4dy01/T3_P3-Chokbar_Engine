#include "BlankProject.h"
#include "Engine/Simulation.h"
#include "Platform/Windows//WinEntry.h"

#include "Core/D3DApp.h"


class Application : public D3DApp {

public:

	Application() {}
	~Application() {}

public:

	void SetupPerGameSettings();

	/* Initialize the application */
	void Initialize() override;

	/* Game Loop */
	void Update(const float dt) override;

	void Render() override;
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
	D3DApp::Initialize();
}

void Application::Update(const float dt)
{
	D3DApp::Update(dt);
}

void Application::Render()
{
	D3DApp::Render();
}