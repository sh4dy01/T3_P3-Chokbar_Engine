#pragma once

namespace Win32 {

	class CHOKBAR_API SubObject {

	public:

		SubObject(WSTRING className, WSTRING titleName, HICON icon);
		~SubObject();



	protected:

		virtual void RegisterNewClass();
		virtual void Initialize() = 0;

		static			LRESULT CALLBACK	SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static			LRESULT				AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual			LRESULT				MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected :

		WSTRING		m_Class;
		WSTRING		m_Title;

		HICON		m_hIcon;
		HWND		m_hWnd;

	};
}
