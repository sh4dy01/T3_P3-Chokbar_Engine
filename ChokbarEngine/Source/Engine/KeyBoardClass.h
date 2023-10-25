#pragma once
#include <queue>

class Keyboard
{
	friend class Window;
public:
	class KeyboardEvent
	{
	public: 
		enum class Type 
		{
			KRealeased,
			KPressed,
			KRepeat,
			Invalid
		};
	private:
		Type m_Type;
		unsigned char m_KeyCode;
	public:
		KeyboardEvent():
			m_Type(Type::Invalid),
			m_KeyCode(0u)
		{}
		KeyboardEvent(Type type, unsigned char keycode) :
			m_Type(type),
			m_KeyCode(keycode)
		{}
		bool IsPressed() const;
		bool IsReleased() const;
		bool IsKeysAutoRepeat() const;
		bool IsValid() const;
		unsigned char GetKeyCode() const;
	};
	public:
		Keyboard();
		Keyboard(const Keyboard&);
		Keyboard& operator=(const Keyboard&);
		bool KeyIsPressed(unsigned char keycode) const;
		KeyboardEvent ReadKey();
		bool KeyIsEmpty() const;
		void FlushKey();
		char ReadChar();
		bool CharIsEmpty() const;
		void FlushChar();
		void Flush();
	private:
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		bool IsKeysAutoRepeat();
		void OnChar(char character);
		void ClearState();
		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);
	private:
		static constexpr unsigned int m_nKeys = 256u;
		static constexpr unsigned int m_nBufferSize = 16u;
		bool m_AutoRepeatEnabled = false;
		bool m_KeyStates[m_nKeys];
		std::queue<KeyboardEvent> m_KeyBuffer;
		std::queue<char> m_CharBuffer;
};
