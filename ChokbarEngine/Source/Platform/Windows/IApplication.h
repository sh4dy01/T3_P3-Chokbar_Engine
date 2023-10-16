#pragma once

#define ENTRYAPP(x) Win32::IApplication* EntryApplication() { return new x; }

namespace Win32
{
	class CHOKBAR_API IApplication
	{

	public:

		IApplication();

		virtual ~IApplication() {};

	public:

		/* Called to set up our per game settings */
		virtual void SetupPerGameSettings() = 0;

		virtual void PreInitialize() = 0;

		/* Initialize the application */
		virtual void Initialize() = 0;

		/* Game Loop */
		virtual void Update() = 0;
	};

	IApplication* EntryApplication();

}
