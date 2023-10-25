#include "Chokbar.h"
#include "Coordinator.h"

namespace Chokbar {

	Coordinator::Coordinator()
		: m_ComponentManager(nullptr), m_EntityManager(nullptr), m_SystemManager(nullptr)
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

	InstanceID Coordinator::CreateNewGameObjectID()
	{
		const InstanceID newId = m_EntityManager->CreateEntity();
		m_ComponentManager->AddComponent<Transform>(newId, Transform());

		return newId;
	}

	InstanceID Coordinator::CreateNewGameObjectID(const Transform& transform)
	{
		const InstanceID newId = m_EntityManager->CreateEntity();
		const Transform copiedTransform = transform;

		m_ComponentManager->AddComponent<Transform>(newId, copiedTransform);

		return newId;
	}

	void Coordinator::UpdateSystems()
	{
		m_SystemManager->UpdateAllSystems();
	}

	void Coordinator::DestroyEntity(InstanceID entity)
	{
		m_EntityManager->DestroyEntity(entity);
		m_SystemManager->EntityDestroyed(entity);
		m_ComponentManager->EntityDestroyed(entity);
	}

}
