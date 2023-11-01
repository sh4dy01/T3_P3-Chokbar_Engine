#pragma once
#include "Engine/ECS/Base/GameObject.h"

class Transform;
class CollisionShape;

class Component
{
public:

	virtual ~Component();

	virtual void OnAddedComponent() {};
	virtual void OnTriggerEnter(CollisionShape* col) {};

	void SetEnabled(bool enabled) { m_isEnabled = enabled; }
	bool IsEnabled() const { return m_isEnabled; }

	Chokbar::GameObject* gameObject;
	Transform* transform;

private:

	bool m_isEnabled = false;

};
