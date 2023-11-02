#pragma once
#include <vector>
#include <Windows.h>
#include <algorithm>


class InputHandler
{
public:
	enum class KeyState
	{
		Up,
		Down,
		Held,
		None
	};

public:

	InputHandler();
	void Init(HWND windowHandle);
	void Update(float dt);

	static bool IsKeyDown(char key);
	static bool IsKeyUp(char key);
	static bool IsKeyHeld(char key);

	static float GetMouseX();
	static float GetMouseY();

	static float GetAxisX();
	static float GetAxisY();

	void CaptureCursor();
	void ReleaseCursor();

	void EnableCursor();
	void DisableCursor();

private:

	void CheckInput();
	void GetNormalizedMovement();

	void HideCursor();
	void ShowCursor();


private:

	bool m_IsFocus;

	static std::vector<char> m_KeyboardInput;
	static std::vector<KeyState> m_KeyStates;

	static POINT m_lastPos;
	static float m_deltaPosX;
	static float m_deltaPosY;

	const int SENSIBILITY = 10;

	float m_timer;
	const float m_mouseRefresh = 0.1f;

	HWND m_WindowHandle;

	bool m_IsEnabled = false;

};