#include "Chokbar.h"
#include "Coordinator.h"

#include "Core/D3D/Internal/MeshRenderer.h"
#include "Engine/ECS/Components/PlayerComponent.h"
#include "Engine/ECS/Systems/PlayerSystem.h"


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

		RegisterComponents();
		RegisterSystems();
	}

	void Coordinator::RegisterComponents()
	{
		//TODO: Automatize this

		RegisterComponent<Transform>();
		RegisterComponent<MeshRenderer>();
		RegisterComponent<PlayerComponent>();
		RegisterComponent<CameraComponent>();
	}

	void Coordinator::RegisterSystems()
	{
		//TODO: Automatize this
		RegisterSystem<PlayerSystem>();
		{
			Signature signature;
			signature.set(GetComponentType<Transform>());
			signature.set(GetComponentType<PlayerComponent>());

			SetSystemSignature<PlayerSystem>(signature);
		}
	}


	InstanceID Coordinator::CreateNewGameObjectWithTransform()
	{
		const InstanceID newId = m_EntityManager->CreateEntity();
		m_ComponentManager->AddComponent<Transform>(newId, new Transform());

		return newId;
	}

	InstanceID Coordinator::CreateNewGameObjectWithTransform(Transform* transform)
	{
		const InstanceID newId = m_EntityManager->CreateEntity();
		Transform* copiedTransform = transform;

		m_ComponentManager->AddComponent<Transform>(newId, copiedTransform);

		return newId;
	}

	void Coordinator::UpdateSystems(float dt)
	{
		m_SystemManager->UpdateAllSystems(dt);
	}

	void Coordinator::DestroyEntity(InstanceID entity)
	{
		m_EntityManager->DestroyEntity(entity);
		m_SystemManager->EntityDestroyed(entity);
		m_ComponentManager->EntityDestroyed(entity);
	}

}
