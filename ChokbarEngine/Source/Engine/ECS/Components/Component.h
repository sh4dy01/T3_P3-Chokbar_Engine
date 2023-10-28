#pragma once

class GameObject;
class Transform;

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
