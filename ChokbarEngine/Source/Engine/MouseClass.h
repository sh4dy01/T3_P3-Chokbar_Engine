#pragma once
#include <queue>

class Mouse
{
	//friend class Window;
public:
	class MouseEvent
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type m_Type;
		bool m_LeftIsPressed = false;
		bool m_RightIsPressed = false;
		bool m_IsInWindow = false;
		int m_X;
		int m_Y;
	public:
		MouseEvent() :
			m_Type(Type::Invalid),
			m_LeftIsPressed(false),
			m_RightIsPressed(false),
			m_X(0),
			m_Y(0)
		{}
		MouseEvent(Type type, const Mouse& parent) :
			m_Type(type),
			m_LeftIsPressed(parent.IsLeftDown()),
			m_RightIsPressed(parent.IsRightDown()),
			m_X(parent.GetPosX()),
			m_Y(parent.GetPosY())
		{}
		bool IsValid() const;
		Type GetType() const;
		std::pair<int, int> GetPos() const;
		int GetPosX() const;
		int GetPosY() const;
		bool IsLeftPressed() const;
		bool IsRightPressed() const;

	};

public:
	Mouse();
	Mouse( const Mouse& );
	Mouse& operator=(const Mouse&);
	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool IsLeftDown() const;
	bool IsRightDown() const;
	bool IsInWindow() const;
	Mouse::MouseEvent Read();
	bool IsEmpty() const;
	void Flush();

public:
	void OnMouseMove(int x, int y);
	void OnMouseLeave();
	void OnMouseEnter();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void TrimBuffer();

private:
	static constexpr unsigned int m_BufferSize = 16u;
	int m_X;
	int m_Y;
	bool m_LeftIsPressed;
	bool m_RightIsPressed;
	bool m_IsInWindow;
	std::queue<MouseEvent> m_Buffer;
};