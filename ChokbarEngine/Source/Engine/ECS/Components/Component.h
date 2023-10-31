#pragma once
#include "Engine/ECS/Base/GameObject.h"

class Transform;

class Component
{
public:
	Chokbar::GameObject *gameObject;
	Transform *transform;
	virtual void OnAddedComponent(){};

	void SetEnabled(bool enabled) { m_isEnabled = enabled; }
	bool IsEnabled() const { return m_isEnabled; }

private:
	bool m_isEnabled = false;
};
