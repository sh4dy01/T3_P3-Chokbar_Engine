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

Chokbar::GameObject* Chokbar::GameObject::Instantiate()
{
	return new GameObject();
}

Chokbar::GameObject* Chokbar::GameObject::Instantiate(const std::string& name)
{
	return new GameObject(name);
}

Chokbar::GameObject* Chokbar::GameObject::Instantiate(GameObject original)
{
	return new GameObject(original);
}

Chokbar::GameObject* Chokbar::GameObject::Instantiate(const GameObject& original, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, Transform parent)
{
	auto go = new GameObject(original);
	go->transform->SetPosition(position);
	go->transform->Rotate(rotation);

	//TODO: Set parent

	return go;
}

