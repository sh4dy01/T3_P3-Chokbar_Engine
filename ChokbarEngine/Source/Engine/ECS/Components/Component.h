#pragma once


namespace Chokbar
{
	class Transform;
	class GameObject;

	class Component
	{
	public:
		GameObject* gameObject;
		Transform* transform;

		void SetEnabled(bool enabled) { m_isEnabled = enabled; }
		bool IsEnabled() const { return m_isEnabled; }

	private:
		bool m_isEnabled = false;
	};
}
