#pragma once

#include "Object.h"
#include "Core/DebugUtils.h"
#include "Engine/ECS/Components/TransformComponent.h"

class Engine;

class GameObject : public Object
{
public:
	GameObject();
	GameObject(const std::string &name);

	template <typename... Component>
	GameObject(const std::string &name, Component... components)
	{
		m_Name = name;
		transform = Engine::GetCoordinator()->GetComponent<Transform>(GetInstanceID());

		(AddComponent<Component>(), ...);
	}

	virtual ~GameObject() override;

	template <class GameObject>
	static GameObject* Instantiate() 
	{
		return  new GameObject();
	}

	template <class GameObject>
	static GameObject* Instantiate(const std::string& name)
	{
		auto go = new GameObject();
		go->SetName(name);

		return go;
	}
	//static Object Instantiate(GameObject original, Transform parent);
	//static Object Instantiate(Object original, Transform parent, bool instantiateInWorldSpace);

	/*
	template <class GameObject>
	static GameObject* Instantiate(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, Transform parent)
	{
		auto go = new GameObject(original);
		go->transform->SetPosition(position);
		go->transform->Rotate(rotation);

		//TODO: Set parent

		return go;
	}*/

	template<class T>
	T* AddComponent()
	{
		auto component = new T();
		component->gameObject = this;
		component->transform = transform;
		component->SetEnabled(true);
		component->OnAddedComponent();

		DEBUG_LOG("Adding component: " + std::string(typeid(T).name()) + " to " + m_Name + " entity");

		Engine::GetCoordinator()->AddComponent<T>(GetInstanceID(), component);

		return component;
	}

	template <class Component>
	Component* AddComponent(Component *component)
	{
		component->gameObject = this;
		component->transform = transform;
		component->SetEnabled(true);
		component->OnAddedComponent();

		DEBUG_LOG("Adding component: " + std::string(typeid(Component).name()) + " to " + m_Name + " entity");

		Engine::GetCoordinator()->AddComponent<Component>(GetInstanceID(), component);

		return component;
	}

	template <class T>
	T *GetComponent()
	{
		return Engine::GetCoordinator()->GetComponent<T>(GetInstanceID());
	}

	template <class T>
	bool HasComponent()
	{
		return Engine::GetCoordinator()->HasComponent<T>(GetInstanceID());
	}

	static GameObject* Find(const std::string& name);

	bool IsActive() const { return m_IsActive; }
	void SetActive(bool value) { m_IsActive = value; }

public:

	Transform *transform;

private:

	bool m_IsActive;

};