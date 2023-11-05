#include "Chokbar.h"
#include "InputHandler.h"


POINT InputHandler::m_lastPos = { 0, 0 };

float InputHandler::m_deltaPosX = 0.0f;
float InputHandler::m_deltaPosY = 0.0f;

std::vector<char> InputHandler::m_KeyboardInput = { 'Z', 'Q', 'S', 'D', VK_SHIFT, VK_SPACE ,VK_LBUTTON, VK_RBUTTON, VK_ESCAPE };
std::vector<InputHandler::KeyState> InputHandler::m_KeyStates = {};


InputHandler::InputHandler()
	: MOUSE_REFRESH_RATE(0.01f), SENSIBILITY(100), m_IsCaptured(false), m_Timer(0.0f), m_WindowHandle(nullptr)
{
	m_KeyStates.reserve(m_KeyboardInput.size());

	for (size_t i = 0; i < m_KeyboardInput.size(); ++i)
	{
		m_KeyStates.push_back(KeyState::None);
	}

	GetCursorPos(&m_lastPos);
}

InputHandler::~InputHandler()
= default;


void InputHandler::Init(HWND windowHandle)
{
	m_WindowHandle = windowHandle;
	m_IsCaptured = false;
	SetCursorToWindowCenter();
}

/// <summary>
/// Updates the input handler, checks for inputs, and updates the mouse refresh timer.
/// </summary>
/// <param name="dt">Delta time since the last frame.</param>
void InputHandler::Update(float dt)
{
	CheckInput();

	m_Timer += dt;
	if (m_Timer > MOUSE_REFRESH_RATE)
	{
		GetNormalizedMovement();

		if (m_IsCaptured)
		{
			SetCursorToWindowCenter();
		}

		m_Timer = 0.0f;
	}
}

void InputHandler::SetCursorToWindowCenter()
{
	RECT rect;
	GetClientRect(m_WindowHandle, &rect);
	POINT windowCenter = { rect.right / 2, rect.bottom / 2 };

	ClientToScreen(m_WindowHandle, &windowCenter);
	SetCursorPos(windowCenter.x, windowCenter.y);
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
	m_IsCaptured = true;

	RECT rect;
	GetClientRect(m_WindowHandle, &rect);
	POINT windowCenter = { rect.right / 2, rect.bottom / 2 };

	ClientToScreen(m_WindowHandle, &windowCenter);

	RECT clipRect = { windowCenter.x - rect.right / 2, windowCenter.y - rect.bottom / 2, windowCenter.x + rect.right / 2, windowCenter.y + rect.bottom / 2 };
	ClipCursor(&clipRect);

	m_lastPos = windowCenter;

	while (ShowCursor(FALSE) >= 0); // Use a loop to ensure the cursor is hidden
}

void InputHandler::ReleaseCursor()
{
	m_IsCaptured = false;

	ClipCursor(nullptr);
	while (ShowCursor(TRUE) < 0); // Use a loop to ensure the cursor is shown

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

	RECT rect;
	GetClientRect(m_WindowHandle, &rect);
	POINT windowCenter = { rect.right / 2, rect.bottom / 2 };

	DirectX::XMFLOAT2 delta;

	delta.x = std::clamp(static_cast<int>(std::abs(windowCenter.x - currentPos.x)), -SENSIBILITY, SENSIBILITY);
	if (currentPos.x < windowCenter.x)
		delta.x *= -1;
	delta.y = std::clamp(static_cast<int>(std::abs(windowCenter.y - currentPos.y)), -SENSIBILITY, SENSIBILITY);
	if (currentPos.y < windowCenter.y)
		delta.y *= -1;

	m_deltaPosX = delta.x / SENSIBILITY;
	m_deltaPosY = delta.y / SENSIBILITY;

	DEBUG_LOG("Delta X: " << m_deltaPosX << " Delta Y: " << m_deltaPosY);
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
