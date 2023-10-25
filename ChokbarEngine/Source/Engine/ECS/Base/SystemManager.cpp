#include "Chokbar.h"
#include "SystemManager.h"

namespace Chokbar {
	SystemManager::SystemManager()
		: m_Signatures(), m_Systems()
	{
	}

	SystemManager::~SystemManager()
	{
		m_Systems.clear();
		m_Signatures.clear();
	}

	void SystemManager::UpdateAllSystems()
	{
		for (auto const& pair : m_Systems)
		{
			auto const& system = pair.second;

			system->Update();
		}
	}


	void SystemManager::EntityDestroyed(Entity entity)
	{
		for (auto const& pair : m_Systems)
		{
			auto const& system = pair.second;

			system->m_AllEntities.erase(entity);
		}
	}

	void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : m_Systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = m_Signatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->m_AllEntities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->m_AllEntities.erase(entity);
			}
		}
	}
}
