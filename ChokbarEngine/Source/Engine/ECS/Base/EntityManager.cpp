#include "Chokbar.h"
#include "EntityManager.h"


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
	CleanEverything();
}

InstanceID EntityManager::GetNewInstanceID()
{
	InstanceID id = m_AvailableEntities.front();
	m_AvailableEntities.pop();

	return id;
}

void EntityManager::RegisterGameObject(GameObject* go)
{
	assert(m_LivingEntityCount < MAX_ENTITIES && "Entities limit exceeded");

	m_LivingEntities[go->GetInstanceID()] = go;

	++m_LivingEntityCount;
}

void EntityManager::DestroyEntity(InstanceID entity)
{
	assert(entity < MAX_ENTITIES && "InstanceID id is out of range");

	delete m_LivingEntities[entity];
	m_LivingEntities[entity] = nullptr;

	m_AllSignatures[entity].reset();	// Reset the signature == reset the entity
	m_AvailableEntities.push(entity);	// entity's id now available

	--m_LivingEntityCount;
}

void EntityManager::CleanEverything() 
{
	for (const auto entity : m_LivingEntities)
	{
		if (!entity) continue;

		InstanceID id = entity->GetInstanceID();

		DestroyEntity(id);
	}

	m_LivingEntityCount = 0;

	for (InstanceID entity = 0; entity < MAX_ENTITIES; ++entity)
	{
		m_AvailableEntities.push(entity);
	}
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

GameObject* EntityManager::GetEntityByName(const std::string& name) const
{
	for (const auto entity : m_LivingEntities)
	{
		if (entity == nullptr) continue;
		if (entity->GetName() == name) 
			return entity;
	}

	return nullptr;
}
