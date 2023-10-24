#pragma once
#include "SubObject.h"
#include "Win32Utils.h"

namespace Win32
{
	class Window : public SubObject
	{
	public:

		Window();
		~Window();

		void CreateNewWindow(int width, int height, const WSTRING& title, HICON icon = nullptr, WindowType type = RESIZABLE);
		void PollEvent();
		virtual bool NeedsToClose() { return needsToClose; }


		int				m_Width;
		int				m_Height;

	protected:

		int m_Width;
		int m_Height;

		WindowType 		m_Type;

		LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	public:

		int			GetWidth() const { return m_Width; }
		int			GetHeight() const { return m_Height; }

		void SetNewSize(int newWidth, int newHeight);

	private:

		bool			needsToClose = false;

	};
}
