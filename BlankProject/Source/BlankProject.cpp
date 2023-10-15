#include "BlankProject.h"
#include "Engine/Simulation.h"
#include "Platform/WIN32/WinEntry.h"


class Application : public Chokbar::Simulation
{

public:
	Application() {}
	~Application() {}

public:

	void SetupPerGameSettings();

	/* Initialize the application */
	void Initialize() {}

	/* Game Loop */
	void Update();
};


ENTRYAPP(Application);

void Application::SetupPerGameSettings()
{
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
}

void Application::Update()
{

}