#include "MouseClass.h"

bool Mouse::MouseEvent::IsValid() const
{
	return m_Type != Type::Invalid;
}

Mouse::MouseEvent::Type Mouse::MouseEvent::GetType() const
{
	return m_Type;
}

std::pair<int, int> Mouse::MouseEvent::GetPos() const
{
	return { m_X, m_Y };
}

int Mouse::MouseEvent::GetPosX() const
{
	return m_X;
}

int Mouse::MouseEvent::GetPosY() const
{
	return m_Y;
}

bool Mouse::MouseEvent::IsLeftPressed() const
{
	return m_LeftIsPressed;
}

bool Mouse::MouseEvent::IsRightPressed() const
{
	return m_RightIsPressed;
}

Mouse::Mouse()
{
}

Mouse::Mouse(const Mouse& other)
{
}

Mouse& Mouse::operator=(const Mouse& other)
{
	return *this;
}


std::pair<int, int> Mouse::GetPos() const
{
	return { m_X, m_Y };
}

int Mouse::GetPosX() const
{
	return m_X;
}

int Mouse::GetPosY() const
{
	return m_Y;
}

bool Mouse::IsLeftDown() const
{
	return m_LeftIsPressed;
}

bool Mouse::IsRightDown() const
{
	return m_RightIsPressed;
}

bool Mouse::IsInWindow() const
{
	return m_IsInWindow;
}

Mouse::MouseEvent Mouse::Read()
{
	if (m_Buffer.size() > 0u)
	{
		MouseEvent e = m_Buffer.front();
		m_Buffer.pop();
		return e;
	}
	return MouseEvent();
}

void Mouse::OnMouseMove(int x, int y)
{
	m_X = x;
	m_Y = y;

	m_Buffer.push(MouseEvent(MouseEvent::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter()
{
	m_IsInWindow = true;

	m_Buffer.push(MouseEvent(MouseEvent::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave()
{
	m_IsInWindow = false;

	m_Buffer.push(MouseEvent(MouseEvent::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y)
{
	m_LeftIsPressed = true;

	m_Buffer.push(MouseEvent(MouseEvent::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y)
{
	m_LeftIsPressed = false;

	m_Buffer.push(MouseEvent(MouseEvent::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y)
{
	m_RightIsPressed = true;

	m_Buffer.push(MouseEvent(MouseEvent::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y)
{
	m_RightIsPressed = false;

	m_Buffer.push(MouseEvent(MouseEvent::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	while (m_Buffer.size() > m_BufferSize)
	{
		m_Buffer.pop();
	}
}

bool Mouse::IsEmpty() const
{
	return m_Buffer.empty();
}

void Mouse::Flush()
{
	m_Buffer = std::queue<MouseEvent>();
}