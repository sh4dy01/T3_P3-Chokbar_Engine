#pragma once

#include "Object.h"
#include <array>
#include <queue>

class EntityManager
{
public:

	// Initialize the queue with all the entities
	EntityManager(); 
	~EntityManager();
	
public:

	InstanceID GetNewInstanceID();
	void RegisterGameObject(GameObject* go);
	void DestroyEntity(InstanceID entity);
	void SetSignature(InstanceID entity, Signature signature);
	Signature GetSignature(InstanceID entity);
	GameObject* GetEntityByName(const std::string& name) const;

	void CleanEverything();


private:

	// A Queue is used because it's the most efficient way to remove the first element and add an element at the end
	std::queue<InstanceID> m_AvailableEntities;

	// Array with fixed size to store the signatures of each entity
	std::array<Signature, MAX_ENTITIES> m_AllSignatures;

	std::array<GameObject*, MAX_ENTITIES> m_LivingEntities = {};

	// Keep track of the number of living entities
	uint32_t m_LivingEntityCount;
};