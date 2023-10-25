#include "Chokbar.h"
#include "GameObject.h"

Chokbar::GameObject::GameObject()
= default;

Chokbar::GameObject::GameObject(const std::string& name)
	: Object(name)
{

}

Chokbar::GameObject::~GameObject()
= default;

void Chokbar::GameObject::AddComponent(Component component) const
{
	Engine::GetCoordinator().AddComponent(m_InstanceID, component);
}


