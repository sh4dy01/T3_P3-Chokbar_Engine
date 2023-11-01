#pragma once

#include "TypeDef.h"

#include <unordered_map>
#include <array>


namespace Chokbar {
	class IComponentArray {

	public:

		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(InstanceID entity) = 0;

	};

	template<class T>
	class ComponentArray : public IComponentArray {

	public:

		void InsertData(InstanceID entity, T* component) {
			size_t newIndex = m_Size;
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;
			m_ComponentArray[newIndex] = component;
			m_Size++;
		}

		void RemoveData(InstanceID entity)
		{
			const size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
			const size_t indexOfLastElement = m_Size - 1;

			delete m_ComponentArray[indexOfRemovedEntity];

			m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			const InstanceID entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
			m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(indexOfLastElement);

			m_Size--;
			
		}



		T* GetData(InstanceID entity)
		{
			if (HasData(entity))
				return m_ComponentArray[m_EntityToIndexMap[entity]];
			else
				return nullptr;
		}

		bool HasData(InstanceID entity) const
		{
			return m_EntityToIndexMap.contains(entity);
		}

		void EntityDestroyed(InstanceID entity) override
		{
			if (m_EntityToIndexMap.contains(entity))
			{
				// Remove the entity's component if it existed
				RemoveData(entity);
			}
		}

		std::array<T*, MAX_ENTITIES>* GetAllData()
		{
		// Get a pointer to a list of all components of type
			return &m_ComponentArray;
		}


	private:

		// The packed array of components (of generic type T),
		// set to a specified maximum amount, matching the maximum number
		// of entities allowed to exist simultaneously, so that each entity
		// has a unique spot.
		std::array<T*, MAX_ENTITIES> m_ComponentArray = {};

		// Map from an entity ID to an array index.
		std::unordered_map<InstanceID, size_t> m_EntityToIndexMap = {};

		// Map from an array index to an entity ID.
		std::unordered_map<size_t, InstanceID> m_IndexToEntityMap = {};

		// Total size of valid entries in the array.
		size_t m_Size = 0;
	};

}
