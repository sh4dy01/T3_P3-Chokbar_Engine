#include "Chokbar.h"
#include "GameObject.h"


Chokbar::GameObject::GameObject()
	: transform(AddComponent<Transform>())
{
}

Chokbar::GameObject::GameObject(const std::string& name)
	: Object(name), transform(AddComponent<Transform>())
{
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

