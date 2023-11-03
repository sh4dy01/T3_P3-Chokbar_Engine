#include "Chokbar.h"
#include "Coordinator.h"

#include "Core/D3D/Internal/MeshRenderer.h"
#include "Engine/ECS/Components/PlayerComponent.h"
#include "Engine/ECS/Components/Collision/Collider.h"
#include "Engine/ECS/Components/Collision/RigidBody.h"
#include "Engine/ECS/Systems/PlayerSystem.h"


Coordinator::Coordinator()
	: m_ComponentManager(nullptr), m_EntityManager(nullptr), m_SystemManager(nullptr)
{
}

Coordinator::~Coordinator()
{
	m_EntityManager.reset();
	m_SystemManager.reset();
	m_ComponentManager.reset();
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
	// TODO: Automatize this

	RegisterComponent<Transform>();
	RegisterComponent<MeshRenderer>();
	RegisterComponent<PlayerComponent>();
	RegisterComponent<Rigidbody>();
	RegisterComponent<SphereCollider>();
	RegisterComponent<CameraComponent>();
}

void Coordinator::RegisterSystems()
{
	// TODO: Automatize this
	RegisterSystem<PlayerSystem>();
	{
		Signature signature;
		signature.set(GetComponentType<Transform>());
		signature.set(GetComponentType<PlayerComponent>());

		SetSystemSignature<PlayerSystem>(signature);
	}
}


InstanceID Coordinator::CreateNewObject(Object* go) const
{		
	return m_EntityManager->RegisterEntity(go);
}

void Coordinator::AwakeComponents()
{
	m_ComponentManager->AwakeAllComponents();
}

void Coordinator::StartComponents()
{
	m_ComponentManager->StartAllComponents();
}

void Coordinator::UpdateComponents()
{
	m_ComponentManager->UpdateAllComponents();
}

void Coordinator::LateUpdateComponents()
{
	m_ComponentManager->LateUpdateAllComponents();
}

void Coordinator::FixedUpdateComponents()
{
	m_ComponentManager->FixedUpdateAllComponents();
}

void Coordinator::RegisterCustomComponent(CustomComponent* customComponent)
{
	m_ComponentManager->RegisterCustomComponent(customComponent);
}

void Coordinator::UnregisterCustomComponent(CustomComponent* customComponent)
{
	m_ComponentManager->UnregisterCustomComponent(customComponent);
}

/*
InstanceID Coordinator::CreateNewObject(Transform* transform)
{
	const auto newGo = m_EntityManager->CreateEntity();
	Transform* copiedTransform = transform;

	m_ComponentManager->AddComponent<Transform>(newId, copiedTransform);

	return newGo;
}
*/

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
