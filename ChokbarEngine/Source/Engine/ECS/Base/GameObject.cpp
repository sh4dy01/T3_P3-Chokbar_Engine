#include "Chokbar.h"
#include "GameObject.h"


Chokbar::GameObject::GameObject()
	: transform(nullptr)
{
	m_InstanceID = Chokbar::Engine::GetInstance()->GetCoordinator()->CreateNewGameObjectWithTransform(this);
}

Chokbar::GameObject::GameObject(const std::string& name)
	: Object(name), transform(nullptr)
{
	m_InstanceID = Chokbar::Engine::GetInstance()->GetCoordinator()->CreateNewGameObjectWithTransform(this);
}


Chokbar::GameObject::~GameObject()
= default;

Chokbar::GameObject* Chokbar::GameObject::Instantiate(GameObject original)
{
	return new GameObject(original);
}

Chokbar::GameObject* Chokbar::GameObject::Instantiate(const GameObject& original, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, Transform parent)
{
	return nullptr;
}

