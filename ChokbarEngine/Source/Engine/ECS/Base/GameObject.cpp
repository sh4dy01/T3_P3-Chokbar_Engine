#include "Chokbar.h"
#include "GameObject.h"

GameObject::GameObject()
{
	transform = Engine::GetCoordinator()->GetComponent<Transform>(m_InstanceID);
}

GameObject::GameObject(const std::string& name)
	: Object(name)
{
	transform = Engine::GetCoordinator()->GetComponent<Transform>(m_InstanceID);
}


GameObject::~GameObject()
= default;



