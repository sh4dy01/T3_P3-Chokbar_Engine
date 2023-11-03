#include "Chokbar.h"
#include "GameObject.h"


GameObject::GameObject()
	: transform(AddComponent<Transform>()), m_IsActive(true)
{
}

GameObject::GameObject(const std::string& name)
	: Object(name), transform(AddComponent<Transform>()), m_IsActive(true)
{
}


GameObject::~GameObject()
{
	transform = nullptr;
}

