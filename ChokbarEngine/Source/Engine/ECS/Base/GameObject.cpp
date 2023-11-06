#include "Chokbar.h"
#include "GameObject.h"


GameObject::GameObject()
	: transform(AddComponent<Transform>()), m_IsActive(true)
{
	Engine::GetCoordinator()->RegisterGameObject(this);
}

GameObject::GameObject(const std::string& name)
	: Object(name), transform(AddComponent<Transform>()), m_IsActive(true)
{
	Engine::GetCoordinator()->RegisterGameObject(this);
}


GameObject::~GameObject()
{
	transform = nullptr;
}

GameObject* GameObject::Find(const std::string& name)
{
	return Engine::GetCoordinator()->GetEntityByName(name);
}

