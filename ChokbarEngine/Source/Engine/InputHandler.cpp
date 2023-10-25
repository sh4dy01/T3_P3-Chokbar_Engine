#include "Chokbar.h"
#include "InputHandler.h"


InputHandler::InputHandler()
{
	m_KeyStates.reserve(m_KeyboardInput.size());
	for (size_t i = 0; i < m_KeyboardInput.size(); ++i)
	{
		m_KeyStates.push_back(KeyState::None);
	}
}

void InputHandler::CheckInput()
{
	for (size_t i = 0; i < m_KeyboardInput.size(); ++i)
	{
		char key = m_KeyboardInput[i];
		bool isKeyDown = GetAsyncKeyState(key) & 0x8000;

		if (isKeyDown)
		{
			if (m_KeyStates[i] == KeyState::None || m_KeyStates[i] == KeyState::Up)
			{
				m_KeyStates[i] = KeyState::Down;
				OutputDebugStringW(L"Key Pressed Down \n");
			}
			else
			{
				m_KeyStates[i] = KeyState::Held;
				OutputDebugStringW(L"Key Pressed Held \n");
			}
		}
		else
		{
			if (m_KeyStates[i] == KeyState::Held || m_KeyStates[i] == KeyState::Down)
			{
				m_KeyStates[i] = KeyState::Up;
				OutputDebugStringW(L"Key Pressed Up \n");
			}
			else
			{
				m_KeyStates[i] = KeyState::None;
			}
		}
	}
}