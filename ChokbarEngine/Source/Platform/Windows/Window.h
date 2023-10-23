#pragma once
#include "SubObject.h"
#include "Win32Utils.h"

namespace Win32
{
	class CHOKBAR_API Window : public SubObject
	{
	public:

		Window(WSTRING title, HICON icon, WindowType type = RESIZABLE);
		Window(WSTRING title, HICON icon, int width, int height, WindowType type = RESIZABLE);
		~Window();


	protected:

		void Initialize() override;
		void PollEvent();
		virtual bool NeedsToClose() { return needsToClose; }


		int				m_Width;
		int				m_Height;

		WindowType 		m_Type;

		LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	public:

		HWND			Handle() const { return m_hWnd; }

		int 			Width() const { return m_Width; }
		int 			Height() const { return m_Height; }

	private:

		bool			needsToClose = false;

	};
}
