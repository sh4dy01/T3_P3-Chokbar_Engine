#include "Chokbar.h"
#include "InputHandler.h"


POINT InputHandler::m_lastPos = { 0, 0 };

float InputHandler::m_deltaPosX = 0.0f;
float InputHandler::m_deltaPosY = 0.0f;

std::vector<char> InputHandler::m_KeyboardInput = { 'Z', 'Q', 'S', 'D', VK_LBUTTON, VK_RBUTTON };
std::vector<InputHandler::KeyState> InputHandler::m_KeyStates = {};


InputHandler::InputHandler()
	: m_WindowHandle(nullptr), m_timer(0.0f), m_mouseRefresh(0.1f)
{
	m_KeyStates.reserve(m_KeyboardInput.size());

	for (size_t i = 0; i < m_KeyboardInput.size(); ++i)
	{
		m_KeyStates.push_back(KeyState::None);
	}

	GetCursorPos(&m_lastPos);
}


void InputHandler::Init(HWND windowHandle)
{
	m_WindowHandle = windowHandle;
}

/// <summary>
/// Updates the input handler, checks for inputs, and updates the mouse refresh timer.
/// </summary>
/// <param name="dt">Delta time since the last frame.</param>
void InputHandler::Update(float dt)
{
	CheckInput();

	m_timer += dt;
	if (m_timer > m_mouseRefresh)
	{
		GetNormalizedMovement();
		m_timer = 0.0f;
	}
}

/// <summary>
/// Checks for key presses and updates the key states.
/// </summary>
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

/// <summary>
/// Determines if a specific key is in the Down state.
/// </summary>
/// <param name="key">The key to check.</param>
/// <returns>True if the key is pressed down, otherwise false.</returns>
bool InputHandler::IsKeyDown(char key)
{
	// [TODO] - Replace char key by an enum
	for (size_t i = 0; i < m_KeyboardInput.size(); ++i)
	{
		if (m_KeyboardInput[i] == toupper(key))
		{
			return m_KeyStates[i] == KeyState::Down;
		}
	}
	return false;
}

/// <summary>
/// Determines if a specific key is in the Up state.
/// </summary>
/// <param name="key">The key to check.</param>
/// <returns>True if the key is up, otherwise false.</returns>
bool InputHandler::IsKeyUp(char key)
{
	for (size_t i = 0; i < m_KeyboardInput.size(); ++i)
	{
		if (m_KeyboardInput[i] == toupper(key))
		{
			return m_KeyStates[i] == KeyState::Up;
		}
	}
	return false;
}

/// <summary>
/// Determines if a specific key is in the Held state.
/// </summary>
/// <param name="key">The key to check.</param>
/// <returns>True if the key is held down, otherwise false.</returns>
bool InputHandler::IsKeyHeld(char key)
{
	for (size_t i = 0; i < m_KeyboardInput.size(); ++i)
	{
		if (m_KeyboardInput[i] == toupper(key))
		{
			return m_KeyStates[i] == KeyState::Held;
		}
	}
	return false;
}

/// <summary>
/// Computes the normalized mouse movement based on sensitivity.
/// </summary>
void InputHandler::GetNormalizedMovement()
{
	POINT currentPos;
	GetCursorPos(&currentPos);

	ScreenToClient(m_WindowHandle, &currentPos);

	float x = std::clamp((int)((currentPos.x - m_lastPos.x)), -SENSIBILITY, SENSIBILITY);
	float y = std::clamp((int)((currentPos.y - m_lastPos.y)), -SENSIBILITY, SENSIBILITY);

	m_deltaPosX = x / SENSIBILITY;
	m_deltaPosY = y / SENSIBILITY;

	m_lastPos = currentPos;
}

/// <summary>
/// Retrieves the normalized x-coordinate of the mouse movement.
/// </summary>
/// <returns>Normalized x-coordinate of the mouse movement.</returns>
float InputHandler::GetMouseX()
{
	return m_lastPos.x;
}

/// <summary>
/// Retrieves the normalized y-coordinate of the mouse movement.
/// </summary>
/// <returns>Normalized y-coordinate of the mouse movement.</returns>
float InputHandler::GetMouseY()
{
	return m_lastPos.y;
}

float InputHandler::GetAxisX()
{
	return m_deltaPosX;
}

float InputHandler::GetAxisY()
{
	return m_deltaPosY;
}
