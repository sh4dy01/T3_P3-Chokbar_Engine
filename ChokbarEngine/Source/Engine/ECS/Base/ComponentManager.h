#pragma once

#include "TypeDef.h"
#include "ComponentArray.h"
#include "Engine/ECS/Components/CustomComponent.h"

class ComponentManager
{
public:

	ComponentManager();
	~ComponentManager();

public:

	void AwakeAllComponents();
	void StartAllComponents();
	void UpdateAllComponents();
	void FixedUpdateAllComponents();
	void LateUpdateAllComponents();

	void RegisterCustomComponent(CustomComponent* customComponent);
	void UnregisterCustomComponent(CustomComponent* customComponent);

	// Notify each component array that an entity has been destroyed
	// If it has a component for that entity, it will remove it

	void CleanEverything();
	void EntityDestroyed(InstanceID entity);

public:

	template<class T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(!m_ComponentTypes.contains(typeName) && "Registering component type more than once.");

		// Add this component type to the component type map
		m_ComponentTypes.insert({ typeName, m_NextComponentType });

		// Create a ComponentArray pointer and add it to the component arrays map
		m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		DEBUG_LOG("Registered " << typeName << " component to system.");

		// Increment the value so that the next component registered will be different
		++m_NextComponentType;
	}

	template<class T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(m_ComponentTypes.contains(typeName) && "Component not registered before use.");

		// Return this component's type - used for creating signatures
		return m_ComponentTypes[typeName];
	}

	template<class T>
	void AddComponent(InstanceID entity, T* component)
	{
		// Add a component to the array for an entity
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<class T>
	void RemoveComponent(InstanceID entity, T* component)
	{
		// Remove a component from the array for an entity
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<class T>
	T* GetComponent(InstanceID entity)
	{
		// Get a pointer to a component from the array for an entity
		return GetComponentArray<T>()->GetData(entity);
	}

	template<class T>
	T* GetAllComponentsOfType()
	{
		// Get a pointer to a list of all components of type
		return GetComponentArray<T>();
	}

	template<class T>
	bool HasComponent(InstanceID entity)
	{
		// Check if an entity has a component by checking if the component type's bit is set
		return GetComponentArray<T>()->HasData(entity);
	}

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<class T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		if (!m_ComponentTypes.contains(typeName))
		{
			RegisterComponent<T>();
		}

		return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
	}

private:

	// Map from type string pointer to a component type
	std::unordered_map<const char*, ComponentType> m_ComponentTypes;

	// Map from type string pointer to a component array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays;

	std::vector<CustomComponent*> m_CustomComponents;

	// The component type to be assigned to the next registered component - starting at 0
	ComponentType m_NextComponentType;

};
