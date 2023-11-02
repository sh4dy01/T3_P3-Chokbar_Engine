#pragma once

class Transform;
class GameObject;

class Component
{
public:

	virtual ~Component();

	virtual void OnAddedComponent() {};

	void SetEnabled(bool enabled) { m_isEnabled = enabled; }
	bool IsEnabled() const { return m_isEnabled; }

	GameObject* gameObject;
	Transform* transform;

private:

	bool m_isEnabled = false;

};
