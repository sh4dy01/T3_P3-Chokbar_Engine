#pragma once

#include "TypeDef.h"

#include <cassert>
#include <unordered_map>
#include <array>


namespace Chokbar {
	class IComponentArray {

	public:

		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;

	};

	template<class T>
	class ComponentArray : public IComponentArray {

	public:

		void InsertData(Entity entity, T component) {
			size_t newIndex = m_Size;
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;
			m_ComponentArray[newIndex] = component;
			m_Size++;
		}

		void RemoveData(Entity entity)
		{
			const size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
			const size_t indexOfLastElement = m_Size - 1;
			m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
			m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(indexOfLastElement);

			m_Size--;
			
		}

		T& GetData(Entity entity)
		{
			assert(m_EntityToIndexMap.contains(entity) && "Retrieving non-existent component.");

			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			if (m_EntityToIndexMap.contains(entity))
			{
				// Remove the entity's component if it existed
				RemoveData(entity);
			}
		}


	private:

		// The packed array of components (of generic type T),
		// set to a specified maximum amount, matching the maximum number
		// of entities allowed to exist simultaneously, so that each entity
		// has a unique spot.
		std::array<T, MAX_ENTITIES> m_ComponentArray = {};

		// Map from an entity ID to an array index.
		std::unordered_map<Entity, size_t> m_EntityToIndexMap = {};

		// Map from an array index to an entity ID.
		std::unordered_map<size_t, Entity> m_IndexToEntityMap = {};

		// Total size of valid entries in the array.
		size_t m_Size = 0;
	};

}
