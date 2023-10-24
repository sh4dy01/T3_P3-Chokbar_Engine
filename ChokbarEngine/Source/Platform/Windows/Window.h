#pragma once
#include "SubObject.h"
#include "Win32Utils.h"

namespace Win32
{
	class Window : public SubObject
	{
	public:

		Window(WSTRING title, HICON icon, WindowType type = RESIZABLE);
		~Window();

		void Initialize() override;
		void PollEvent();


		virtual bool NeedsToClose() { return needsToClose; }

	protected:

		SIZE			m_Size;
		WindowType 		m_Type;

		LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	public:

		SIZE			Size() { return m_Size; }

		void			Size(SIZE size) { m_Size = size; }
		void			Size(INT cx, INT cy) { m_Size.cx = cx; m_Size.cy = cy; }

	private:

		bool			needsToClose = false;

	};
}
