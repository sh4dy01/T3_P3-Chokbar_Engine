#pragma once
#include "Win32Utils.h"
#include "Engine/MouseClass.h"
#include "Engine/KeyBoardClass.h"

namespace Win32
{
	class Window
	{
	public:
		Window();
		~Window();

		void CreateNewWindow(int width, int height, const WSTRING &title, HICON icon = nullptr, WindowType type = RESIZABLE);
		void PollEvent();
		bool NeedsToClose() const { return needsToClose; }

	protected:
		static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void RegisterNewClass();

	protected:
		int m_Width;
		int m_Height;

		WindowType m_Type;
		HWND m_Hwnd;
		WSTRING m_Title;
		HICON m_hIcon;

	private:
		bool needsToClose = false;

	public:
		Mouse m_Mouse;
		Keyboard m_Keyboard;
	};
}
