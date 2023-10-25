#include "KeyBoardClass.h"

bool Keyboard::KeyboardEvent::IsPressed() const
{
	return m_Type == Type::KPressed;
}

bool Keyboard::KeyboardEvent::IsReleased() const
{
	return m_Type == Type::KRealeased;
}

bool Keyboard::KeyboardEvent::IsKeysAutoRepeat() const
{
	return m_Type == Type::KRepeat;
}

bool Keyboard::KeyboardEvent::IsValid() const
{
	return m_Type != Type::Invalid;
}

unsigned char Keyboard::KeyboardEvent::GetKeyCode() const
{
	return m_KeyCode;
}

Keyboard::Keyboard()
{
}

Keyboard::Keyboard(const Keyboard& other)
{
}

Keyboard& Keyboard::operator=(const Keyboard& other)
{
	return *this;
}

bool Keyboard::KeyIsPressed(unsigned char keycode) const
{
	return m_KeyStates[keycode];
}

Keyboard::KeyboardEvent Keyboard::ReadKey()
{
	if (m_KeyBuffer.size() > 0u)
	{
		KeyboardEvent e = m_KeyBuffer.front();
		m_KeyBuffer.pop();
		return e;
	}
	else
	{
		return KeyboardEvent();
	}
}

bool Keyboard::KeyIsEmpty() const
{
	return m_KeyBuffer.empty();
}

void Keyboard::FlushKey()
{
	m_KeyBuffer = std::queue<KeyboardEvent>();
}

char Keyboard::ReadChar()
{
	if (m_CharBuffer.size() > 0u)
	{
		unsigned char charcode = m_CharBuffer.front();
		m_CharBuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const
{
	return m_CharBuffer.empty();
}

void Keyboard::FlushChar()
{
	m_CharBuffer = std::queue<char>();
}

void Keyboard::Flush()
{
	FlushKey();
	FlushChar();
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{
	m_KeyStates[keycode] = true;
	m_KeyBuffer.push(KeyboardEvent(KeyboardEvent::Type::KPressed, keycode));
	TrimBuffer(m_KeyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	m_KeyStates[keycode] = false;
	m_KeyBuffer.push(KeyboardEvent(KeyboardEvent::Type::KRealeased, keycode));
	TrimBuffer(m_KeyBuffer);
}

bool Keyboard::IsKeysAutoRepeat()
{
	return m_AutoRepeatEnabled;
}

void Keyboard::OnChar(char character)
{
	m_CharBuffer.push(character);
	TrimBuffer(m_CharBuffer);
}

void Keyboard::ClearState()
{
	for (int i = 0; i < 256; i++)
	{
		m_KeyStates[i] = false;
	}
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > m_nBufferSize)
	{
		buffer.pop();
	}
}