#pragma once

#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"


class Coordinator
{
public:
	Coordinator();
	~Coordinator();

public:
	// Initialize the coordinator and all its managers
	void Init();


	// Create a new game object ID
	InstanceID GetNewInstanceID() const;
	void RegisterGameObject(GameObject* go) const;


	// Create a new game object ID
	//GameObject* CreateNewObject(Transform* transform);

	void AwakeComponents();
	void StartComponents();
	void UpdateComponents();
	void LateUpdateComponents();
	void FixedUpdateComponents();

	void RegisterCustomComponent(CustomComponent* customComponent);
	void UnregisterCustomComponent(CustomComponent* customComponent);

	void RegisterDestroyedEntity(InstanceID entity);

	void DestroyRegisteredEntites();

	void UpdateSystems(float dt);


private:

	void RegisterComponents();
	void RegisterSystems();

	void DestroyEntity(InstanceID entity);

public:

#pragma region Component methods

	template <typename T>
	void RegisterComponent() const
	{
		m_ComponentManager->RegisterComponent<T>();
	}

	template <typename T>
	void AddComponent(InstanceID entity, T *component)
	{
		m_ComponentManager->AddComponent<T>(entity, component);

		auto signature = m_EntityManager->GetSignature(entity);
		signature.set(m_ComponentManager->GetComponentType<T>(), true);
		m_EntityManager->SetSignature(entity, signature);

		m_SystemManager->EntitySignatureChanged(entity, signature);
	}

	template <typename T>
	void RemoveComponent(InstanceID entity, T* component) const
	{
		m_ComponentManager->RemoveComponent<T>(entity);

		auto signature = m_EntityManager->GetSignature(entity);
		signature.set(m_ComponentManager->GetComponentType<T>(), false);
		m_EntityManager->SetSignature(entity, signature);

		m_SystemManager->EntitySignatureChanged(entity, signature);
	}

	template <typename T>
	T *GetComponent(InstanceID entity)
	{
		return m_ComponentManager->GetComponent<T>(entity);
	}

	template <typename T>
	bool HasComponent(InstanceID entity) const
	{
		return m_ComponentManager->HasComponent<T>(entity);
	}

	template <typename T>
	ComponentType GetComponentType() const
	{
		return m_ComponentManager->GetComponentType<T>();
	}

	template <class T>
	std::shared_ptr<ComponentArray<T>> GetAllComponentsOfType()
	{
		// Get a pointer to a list of all components of type
		return m_ComponentManager->GetComponentArray<T>();
	}

#pragma endregion

#pragma region Entity methods

	GameObject* GetEntityByName(const std::string& name) const;


#pragma endregion

#pragma region System methods

	template <typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return m_SystemManager->RegisterSystem<T>();
	}

	template <typename T>
	void SetSystemSignature(Signature signature) const
	{
		m_SystemManager->SetSignature<T>(signature);
	}

#pragma endregion

private:

	std::vector<InstanceID> m_EntitesToUpdate;
	std::vector<InstanceID> m_EntitiesToDestroy;

	std::unique_ptr<ComponentManager> m_ComponentManager;
	std::unique_ptr<EntityManager> m_EntityManager;
	std::unique_ptr<SystemManager> m_SystemManager;
};