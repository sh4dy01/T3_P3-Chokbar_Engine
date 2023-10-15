#pragma once

#include "Platform/Win32/IApplication.h"
#include "Core/D3DApp.h"

namespace Chokbar {

	class CHOKBAR_API Simulation : public Win32::IApplication, public D3DApp {

	public:

		Simulation();
		~Simulation();

		void PreInitialize() override;

		LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	};
}