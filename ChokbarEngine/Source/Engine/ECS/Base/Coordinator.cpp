#include "Chokbar.h"
#include "Coordinator.h"

#include "D3D/Renderers/MeshRenderer.h"
#include "D3D/Renderers/ParticleRenderer.h"
#include "D3D/Renderers/SkyRenderer.h"
#include "D3D/Renderers/UIRenderer.h"
#include "Engine/ECS/Components/Collision/Collider.h"
#include "Engine/ECS/Components/Collision/RigidBody.h"



Coordinator* Coordinator::m_Instance = nullptr;

Coordinator::Coordinator()
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
	RegisterComponent<Rigidbody>();
	RegisterComponent<SphereCollider>();
	RegisterComponent<CameraComponent>();
	RegisterComponent<ParticleRenderer>();
	RegisterComponent<SkyRenderer>();
	RegisterComponent<UIRenderer>();
}

void Coordinator::RegisterSystems()
{


}

GameObject* Coordinator::GetEntityByName(const std::string& name)
{
	return m_EntityManager->GetEntityByName(name);
}


Coordinator* Coordinator::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new Coordinator();
	}

	return m_Instance;
}

InstanceID Coordinator::GetNewInstanceID()
{
	return m_EntityManager->GetNewInstanceID();
}

void Coordinator::RegisterGameObject(GameObject* go)
{
	return m_EntityManager->RegisterGameObject(go);
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

void Coordinator::CleanUp() 
{
	m_ComponentManager->CleanEverything();
	m_EntityManager->CleanEverything();
}

void Coordinator::RegisterCustomComponent(CustomComponent* customComponent)
{
	m_ComponentManager->RegisterCustomComponent(customComponent);
}

void Coordinator::UnregisterCustomComponent(CustomComponent* customComponent)
{
	m_ComponentManager->UnregisterCustomComponent(customComponent);
}

void Coordinator::SetEntityToBeDestroyed(InstanceID entity)
{
	m_EntitiesToDestroy.push_back(entity);
}

void Coordinator::DestroyRegisteredEntites()
{
	if (m_EntitiesToDestroy.empty()) return;

	for (const auto entity : m_EntitiesToDestroy)
	{
		DestroyEntity(entity);
	}

	m_EntitiesToDestroy.clear();
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