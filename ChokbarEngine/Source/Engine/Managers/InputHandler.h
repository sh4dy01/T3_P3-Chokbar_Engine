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

	bool IsKeyDown(char key) const;
	bool IsKeyUp(char key) const;
	bool IsKeyHeld(char key) const;

	float GetMouseX() const;
	float GetMouseY() const;

	float GetAxisX() const;
	float GetAxisY() const;

private:

	void CheckInput();
	void GetNormalizedMovement();

private:

	std::vector<char> m_KeyboardInput = { 'Z', 'Q', 'S', 'D', VK_LBUTTON, VK_RBUTTON };
	std::vector<KeyState> m_KeyStates;

	POINT m_lastPos;
	float m_deltaPosX;
	float m_deltaPosY;

	const int SENSIBILITY = 100;

	float m_timer;
	const float m_mouseRefresh = 0.1f;

	HWND m_WindowHandle;

};