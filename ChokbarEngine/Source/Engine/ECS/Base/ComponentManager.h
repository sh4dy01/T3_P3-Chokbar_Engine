#pragma once

#include "TypeDef.h"
#include "ComponentArray.h"
#include "Core/D3D/Internal/MeshRenderer.h"

namespace Chokbar {

	class ComponentManager
	{
	public:

		ComponentManager();
		~ComponentManager();

	public:

		template<class T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Registering component type more than once.");

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
		void AddComponent(InstanceID entity, T component)
		{
			const char* typeName = typeid(T).name();

			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<class T>
		void RemoveComponent(InstanceID entity)
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

		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		void EntityDestroyed(InstanceID entity);

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<class T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}

	private:

		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> m_ComponentTypes;

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays;

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType m_NextComponentType;

	};
}