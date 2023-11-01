#include "Chokbar.h"
#include "InputHandler.h"


POINT InputHandler::m_lastPos = { 0, 0 };

float InputHandler::m_deltaPosX = 0.0f;
float InputHandler::m_deltaPosY = 0.0f;

std::vector<char> InputHandler::m_KeyboardInput = { 'Z', 'Q', 'S', 'D', VK_SHIFT, VK_SPACE ,VK_LBUTTON, VK_RBUTTON };
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
			}
			else
			{
				m_KeyStates[i] = KeyState::Held;
			}
		}
		else
		{
			if (m_KeyStates[i] == KeyState::Held || m_KeyStates[i] == KeyState::Down)
			{
				m_KeyStates[i] = KeyState::Up;
			}
			else
			{
				m_KeyStates[i] = KeyState::None;
			}
		}
	}
}

void InputHandler::CaptureCursor()
{
	RECT rect;
	GetClientRect(m_WindowHandle, &rect);
	POINT windowCenter = { rect.right / 2, rect.bottom / 2 };

	ClientToScreen(m_WindowHandle, &windowCenter);

	SetCursorPos(windowCenter.x, windowCenter.y);

	RECT clipRect = { windowCenter.x, windowCenter.y, windowCenter.x + 1, windowCenter.y + 1 };
	ClipCursor(&clipRect);

	m_lastPos = windowCenter;
}

void InputHandler::ReleaseCursor()
{
	ClipCursor(nullptr); // Libérez le mouvement du curseur.
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
	// Déterminez le centre de la fenêtre.
	RECT rect;
	GetClientRect(m_WindowHandle, &rect);
	POINT windowCenter = { rect.right / 2, rect.bottom / 2 };

	// Convertissez le point du centre en coordonnées écran pour SetCursorPos.
	ClientToScreen(m_WindowHandle, &windowCenter);

	// Obtenez la position actuelle de la souris.
	POINT currentPos;
	GetCursorPos(&currentPos);
	ScreenToClient(m_WindowHandle, &currentPos);

	// Calculez le mouvement delta.
	float x = static_cast<float>(currentPos.x - windowCenter.x);
	float y = static_cast<float>(currentPos.y - windowCenter.y);

	// Clamp the delta movements with sensitivity.
	x = std::clamp(x, (float) -SENSIBILITY, (float)SENSIBILITY);
	y = std::clamp(y, (float)-SENSIBILITY, (float)SENSIBILITY);

	// Update the member variables for delta positions.
	m_deltaPosX = x / SENSIBILITY;
	m_deltaPosY = y / SENSIBILITY;

	// Remettez le curseur au centre de la fenêtre.
	SetCursorPos(windowCenter.x, windowCenter.y);

	// Mettez à jour la dernière position connue de la souris avec le centre de la fenêtre.
	m_lastPos = windowCenter;
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
