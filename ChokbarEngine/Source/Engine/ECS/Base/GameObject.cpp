#include "Chokbar.h"
#include "GameObject.h"

GameObject::GameObject()
{
	transform = Chokbar::Engine::GetCoordinator().GetComponent<Transform>(m_InstanceID);
}

GameObject::GameObject(const std::string& name)
	: Object(name)
{
	transform = Chokbar::Engine::GetCoordinator().GetComponent<Transform>(m_InstanceID);
}


GameObject::~GameObject()
= default;



