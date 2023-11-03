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
		transform = Engine::GetCoordinator()->GetComponent<Transform>(m_InstanceID);

		(AddComponent<Component>(), ...);
	}

	~GameObject();

	static GameObject* Instantiate();
	static GameObject* Instantiate(const std::string& name);
	static GameObject* Instantiate(GameObject original);
	//static Object Instantiate(GameObject original, Transform parent);
	//static Object Instantiate(Object original, Transform parent, bool instantiateInWorldSpace);
	static GameObject* Instantiate(const GameObject& original, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, Transform parent);

	template<class Component>
	Component* AddComponent()
	{
		auto component = new Component();
		component->gameObject = this;
		component->transform = transform;
		component->SetEnabled(true);
		component->OnAddedComponent();

		DEBUG_LOG("Adding component: " + std::string(typeid(Component).name()) + " to " + m_Name + " entity");

		Engine::GetCoordinator()->AddComponent<Component>(m_InstanceID, component);

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

		Engine::GetCoordinator()->AddComponent<Component>(m_InstanceID, component);

		return component;
	}

	template <class T>
	T *GetComponent()
	{
		return Engine::GetCoordinator()->GetComponent<T>(m_InstanceID);
	}

	template <class T>
	bool HasComponent()
	{
		return Engine::GetCoordinator()->HasComponent<T>(m_InstanceID);
	}

	bool IsActive() const { return m_IsActive; }
	void SetActive(bool value) { m_IsActive = value; }

public:

	Transform *transform;

private:

	bool m_IsActive;

};