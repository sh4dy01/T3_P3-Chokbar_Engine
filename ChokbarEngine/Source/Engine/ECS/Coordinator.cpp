#include "Chokbar.h"
#include "Coordinator.h"

namespace Chokbar {

	Coordinator::Coordinator()
		: m_EntityManager(nullptr), m_SystemManager(nullptr), m_ComponentManager(nullptr)
	{
	}

	Coordinator::~Coordinator()
	{
		m_EntityManager.release();
		m_SystemManager.release();
		m_ComponentManager.release();
	}

	void Coordinator::Init()
	{
		// Create pointers to each manager
		m_ComponentManager = std::make_unique<ComponentManager>();
		m_EntityManager = std::make_unique<EntityManager>();
		m_SystemManager = std::make_unique<SystemManager>();
	}

	Entity Coordinator::CreateEntity()
	{
		m_EntityManager->CreateEntity();
	}

	void Coordinator::DestroyEntity(Entity entity)
	{
		m_EntityManager->DestroyEntity(entity);
		m_SystemManager->EntityDestroyed(entity);
		m_ComponentManager->EntityDestroyed(entity);
	}

}
