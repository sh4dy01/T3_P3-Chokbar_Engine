#pragma once

#include "Platform/Windows/IApplication.h"

namespace Chokbar {

	class CHOKBAR_API Simulation : public Win32::IApplication, public Win32::Window {

	public:

		Simulation();
		~Simulation();

		void PreInitialize() override;


		LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	};
}