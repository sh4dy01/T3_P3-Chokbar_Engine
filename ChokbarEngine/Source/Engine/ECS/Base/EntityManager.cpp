#include "Chokbar.h"
#include "EntityManager.h"

#include <cassert>


namespace Chokbar {

	EntityManager::EntityManager()
		: m_LivingEntityCount(0)
	{
		for (InstanceID entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntities.push(entity);
		}
	}

	EntityManager::~EntityManager()
	{
	}

	InstanceID EntityManager::CreateEntity(GameObject* go)
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Entities limit exceeded");

		InstanceID id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		m_LivingEntities.push_back(go);

		++m_LivingEntityCount;

		return id;
	}

	void EntityManager::DestroyEntity(InstanceID entity, GameObject* go)
	{
		assert(entity < MAX_ENTITIES && "InstanceID id is out of range");

		m_AllSignatures[entity].reset();	// Reset the signature == reset the entity
		m_AvailableEntities.push(entity);	// entity's id now available
		m_LivingEntities.erase(std::remove(m_LivingEntities.begin(), m_LivingEntities.end(), go), m_LivingEntities.end());

		delete go;
		go = nullptr;

		--m_LivingEntityCount;
	}

	void EntityManager::SetSignature(InstanceID entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "InstanceID id is out of range");

		m_AllSignatures[entity] = signature;
	}

	Signature EntityManager::GetSignature(InstanceID entity)
	{
		assert(entity < MAX_ENTITIES && "InstanceID id is out of range");

		return m_AllSignatures[entity];
	}

}
