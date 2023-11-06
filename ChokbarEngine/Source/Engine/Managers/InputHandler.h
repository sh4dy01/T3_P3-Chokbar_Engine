#pragma once
#include <Windows.h>
#include <vector>

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
	~InputHandler();

	void Init(HWND windowHandle);
	void Update(float dt);

	void SetCursorToWindowCenter();

	static bool IsKeyDown(char key);
	static bool IsKeyUp(char key);
	static bool IsKeyHeld(char key);

	static float GetMouseX();
	static float GetMouseY();

	static float GetAxisX();
	static float GetAxisY();

	void CaptureCursor();
	void ReleaseCursor();


private:

	void CheckInput();
	void GetNormalizedMovement();

private:

	const float MOUSE_REFRESH_RATE;
	const int SENSIBILITY = 10;

	bool m_IsCaptured;

	static std::vector<char> m_KeyboardInput;
	static std::vector<KeyState> m_KeyStates;

	static POINT m_lastPos;
	static float m_deltaPosX;
	static float m_deltaPosY;

	float m_Timer;

	HWND m_WindowHandle;
};