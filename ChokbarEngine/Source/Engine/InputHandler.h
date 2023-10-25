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


private:

	std::vector<char> m_KeyboardInput = { 'Z', 'Q', 'S', 'D', VK_LBUTTON, VK_RBUTTON };
	std::vector<KeyState> m_KeyStates;

};