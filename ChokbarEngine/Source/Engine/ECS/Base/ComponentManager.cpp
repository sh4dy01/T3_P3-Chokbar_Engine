#include "Chokbar.h"
#include "ComponentManager.h"


ComponentManager::ComponentManager()
	: m_NextComponentType(0)
{
	m_ComponentArrays = {};
	m_ComponentTypes = {};
}

ComponentManager::~ComponentManager()
{
	for (auto customComponent : m_CustomComponents)
	{
		customComponent = nullptr;
	}
}

void ComponentManager::AwakeAllComponents()
{
	for (auto const customComponent : m_CustomComponents)
	{
		customComponent->Awake();
	}
}

void ComponentManager::StartAllComponents()
{
	for (auto const customComponent : m_CustomComponents)
	{
		if (!customComponent->gameObject->IsActive() || !customComponent->IsEnabled()) continue;

		customComponent->Start();
	}
}

void ComponentManager::UpdateAllComponents()
{
	const int size = m_CustomComponents.size();
	for (int i = 0; i < size; i++)
	{
		auto const customComponent = m_CustomComponents[i];

		if (!customComponent->gameObject->IsActive() || !customComponent->IsEnabled()) continue;

		customComponent->Update();
	}
}

void ComponentManager::FixedUpdateAllComponents()
{
	for (auto const customComponent : m_CustomComponents)
	{
		if (!customComponent->gameObject->IsActive() || !customComponent->IsEnabled()) continue;

		customComponent->FixedUpdate();
	}
}

void ComponentManager::LateUpdateAllComponents()
{
	for (auto const customComponent : m_CustomComponents)
	{
		if (!customComponent->gameObject->IsActive() || !customComponent->IsEnabled()) continue;

		customComponent->LateUpdate();
	}
}

void ComponentManager::RegisterCustomComponent(CustomComponent* customComponent)
{
	m_CustomComponents.push_back(customComponent);
}

void ComponentManager::UnregisterCustomComponent(CustomComponent* customComponent)
{
	std::erase(m_CustomComponents, customComponent);
}

void ComponentManager::EntityDestroyed(InstanceID entity)
{
	for (auto const& pair : m_ComponentArrays)
	{
		auto const& component = pair.second;

		component->EntityDestroyed(entity);
	}
}

