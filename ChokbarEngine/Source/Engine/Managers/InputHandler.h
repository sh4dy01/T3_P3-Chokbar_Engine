#pragma once
#include <vector>
#include <Windows.h>
#include <algorithm>


static class InputHandler
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

private:

	void CheckInput();
	void GetNormalizedMovement();

private:

	static std::vector<char> m_KeyboardInput;
	static std::vector<KeyState> m_KeyStates;

	static POINT m_lastPos;
	static float m_deltaPosX;
	static float m_deltaPosY;

	const int SENSIBILITY = 100;

	float m_timer;
	const float m_mouseRefresh = 0.1f;

	HWND m_WindowHandle;

};