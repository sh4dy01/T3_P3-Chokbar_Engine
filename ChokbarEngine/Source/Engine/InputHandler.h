#pragma once
#include <vector>
#include <Windows.h>


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

	void CheckInput();

	bool IsKeyDown(char key) const;
	bool IsKeyUp(char key) const;
	bool IsKeyHeld(char key) const;

private:

	std::vector<char> m_KeyboardInput = { 'Z', 'Q', 'S', 'D', VK_LBUTTON, VK_RBUTTON };
	std::vector<KeyState> m_KeyStates;

};