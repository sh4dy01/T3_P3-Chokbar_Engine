#pragma once

class Transform;
class GameObject;

class Component
{
public:

	Component();
	virtual ~Component();

	virtual void OnAddedComponent() {};
	virtual void OnRemovedComponent() {};

	void SetEnabled(bool enabled) { m_isEnabled = enabled; }
	void SetInitialized() { m_isInitialized = true; }

	bool IsEnabled() const { return m_isEnabled; }
	bool IsInitialized() const { return m_isInitialized; }
	bool IsCustomComponent() const { return m_isCustomComponent; }

public:

	GameObject* gameObject;
	Transform* transform;

protected:

	bool m_isCustomComponent;

private:

	bool m_isEnabled;
	bool m_isInitialized;

};
