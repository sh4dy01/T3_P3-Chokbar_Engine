#pragma once

#include "Object.h"
#include "Engine/Engine.h"
#include "Engine/ECS/Components/Component.h"


class GameObject : public Object
{
public:

	GameObject();
	GameObject(const std::string& name);

	template<typename... Component>
	GameObject(const std::string& name, Component... components)
	{
		m_Name = name;
		transform = Chokbar::Engine::GetCoordinator().GetComponent<Transform>(m_InstanceID);


		(AddComponent<Component>(), ...);       
	}

	~GameObject();

	template<class Component>
	void AddComponent()
	{
		Component component;
		component.gameObject = this;
		component.transform = this->transform;

		DEBUG_LOG("Adding component: " + std::string(typeid(Component).name()) + " to " + m_Name + " entity");

		Chokbar::Engine::GetCoordinator().AddComponent<Component>(m_InstanceID, component);
	}

	template<class T>
	T* GetComponent()
	{
		return Chokbar::Engine::GetCoordinator().GetComponent<T>(m_InstanceID);
	}

	template<class T>
	bool HasComponent()
	{
		return Chokbar::Engine::GetCoordinator().HasComponent<T>(m_InstanceID);
	}

public:

	Transform* transform;
};



