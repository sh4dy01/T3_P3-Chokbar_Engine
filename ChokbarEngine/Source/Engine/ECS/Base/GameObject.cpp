#include "Chokbar.h"
#include "GameObject.h"


GameObject::GameObject()
	: transform(AddComponent<Transform>())
{
}

GameObject::GameObject(const std::string& name)
	: Object(name), transform(AddComponent<Transform>())
{
}


GameObject::~GameObject()
{
	transform = nullptr;
}

GameObject* GameObject::Instantiate()
{
	return new GameObject();
}

GameObject* GameObject::Instantiate(const std::string& name)
{
	return new GameObject(name);
}

GameObject* GameObject::Instantiate(GameObject original)
{
	return new GameObject(original);
}

GameObject* GameObject::Instantiate(const GameObject& original, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, Transform parent)
{
	auto go = new GameObject(original);
	go->transform->SetPosition(position);
	go->transform->Rotate(rotation);

	//TODO: Set parent

	return go;
}

