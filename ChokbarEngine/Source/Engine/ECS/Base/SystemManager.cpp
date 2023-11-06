#include "Chokbar.h"
#include "SystemManager.h"

SystemManager::SystemManager()
	: m_Signatures(), m_Systems()
{
}

SystemManager::~SystemManager()
{
	m_Systems.clear();
	m_Signatures.clear();
}

void SystemManager::UpdateAllSystems(float dt)
{
	for (auto const& pair : m_Systems)
	{
		auto const& system = pair.second;

		system->Update(dt);
	}
}


void SystemManager::EntityDestroyed(InstanceID entity)
{
	for (auto const& pair : m_Systems)
	{
		auto const& system = pair.second;

		system->m_RegisteredEntities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(InstanceID entity, Signature entitySignature)
{
	// Notify each system that an entity's signature changed
	for (auto const& pair : m_Systems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = m_Signatures[type];

		// InstanceID signature matches system signature - insert into set
		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->m_RegisteredEntities.insert(entity);
		}
		// InstanceID signature does not match system signature - erase from set
		else
		{
			system->m_RegisteredEntities.erase(entity);
		}
	}
}