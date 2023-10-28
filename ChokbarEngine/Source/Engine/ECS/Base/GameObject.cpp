#include "Chokbar.h"
#include "GameObject.h"


Chokbar::GameObject::GameObject()
{
	transform = Engine::GetCoordinator()->GetComponent<Transform>(m_InstanceID);
}

Chokbar::GameObject::GameObject(const std::string& name)
	: Object(name)
{
	transform = Engine::GetCoordinator()->GetComponent<Transform>(m_InstanceID);
}


Chokbar::GameObject::~GameObject()
= default;

