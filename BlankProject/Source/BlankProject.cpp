#include "BlankProject.h"

using namespace Chokbar;

class Application : public Win32::IApplication {

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
	GameObject test = GameObject("player");
	GameObject player = GameObject("player", PlayerComponent(), MeshRenderer());
}

void Application::Update(const float dt)
{
	
}

void Application::Shutdown()
{

}