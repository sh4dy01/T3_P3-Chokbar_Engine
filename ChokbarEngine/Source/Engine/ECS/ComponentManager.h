#pragma once

#include "TypeDef.h"
#include "ComponentArray.h"
#include "Component.h"


namespace Chokbar {

	class ComponentManager
	{
	public:

		ComponentManager();
		~ComponentManager();

	public:

		template<class IComponent>
		void RegisterComponent()
		{
			const char* typeName = typeid(IComponent).name();

			assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Registering component type more than once.");

			// Add this component type to the component type map
			m_ComponentTypes.insert({ typeName, m_NextComponentType });

			// Create a ComponentArray pointer and add it to the component arrays map
			m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			// Increment the value so that the next component registered will be different
			++m_NextComponentType;
		}

		template<class IComponent>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(IComponent).name();

			assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return m_ComponentTypes[typeName];
		}

		template<class IComponent>
		void AddComponent(Entity entity, IComponent component)
		{
			// Add a component to the array for an entity
			GetComponentArray<IComponent>()->InsertData(entity, component);
		}

		template<class T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<IComponent>()->RemoveData(entity);
		}

		template<class IComponent>
		IComponent& GetComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<IComponent>()->GetData(entity);
		}

		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		void EntityDestroyed(Entity entity);

	private:

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<class IComponent>
		std::shared_ptr<ComponentArray<IComponent>> GetComponentArray()
		{
			const char* typeName = typeid(IComponent).name();

			assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<IComponent>>(m_ComponentArrays[typeName]);
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