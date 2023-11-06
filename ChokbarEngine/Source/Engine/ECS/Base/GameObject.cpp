#include "Chokbar.h"
#include "GameObject.h"

#include "Engine/Engine.h"

GameObject::GameObject()
	: transform(AddComponent<Transform>()), m_IsActive(true)
{
	Coordinator::GetInstance()->RegisterGameObject(this);
}

GameObject::GameObject(const std::string& name)
	: Object(name), transform(AddComponent<Transform>()), m_IsActive(true)
{
	Coordinator::GetInstance()->RegisterGameObject(this);
}


GameObject::~GameObject()
{
	transform = nullptr;
}

GameObject* GameObject::Find(const std::string& name)
{
	return Coordinator::GetInstance()->GetEntityByName(name);
}

