#include "Chokbar.h"
#include "EntityManager.h"

#include <cassert>


namespace Chokbar {

	EntityManager::EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntities.push(entity);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		assert(m_LivingEntityCoount < MAX_ENTITIES && "Entities limit exceeded");

		Entity id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		++m_LivingEntityCoount;

		return id;
	}

	void EntityManager::DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity id is out of range");

		m_AllSignatures[entity].reset();	// Reset the signature == reset the entity
		m_AvailableEntities.push(entity);	// entity's id now available
		--m_LivingEntityCoount;
	}

	void EntityManager::SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity id is out of range");

		m_AllSignatures[entity] = signature;
	}

	Signature EntityManager::GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity id is out of range");

		return m_AllSignatures[entity];
	}

}
