#include "Chokbar.h"
#include "ComponentManager.h"

namespace Chokbar {
	ComponentManager::ComponentManager()
		: m_ComponentArrays(), m_ComponentTypes(), m_NextComponentType(0)
	{
	}

	ComponentManager::~ComponentManager()
	{
	}

	void ComponentManager::EntityDestroyed(Entity entity)
	{
		for (auto const& pair : m_ComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}
}

