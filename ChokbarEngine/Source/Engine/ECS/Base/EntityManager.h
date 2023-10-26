#pragma once

#include "TypeDef.h"

#include <array>
#include <queue>


namespace Chokbar {

	class EntityManager
	{
	public:

		// Initialize the queue with all the entities
		EntityManager(); 
		~EntityManager();
	
	public:

		InstanceID CreateEntity();
		void DestroyEntity(InstanceID entity);
		void SetSignature(InstanceID entity, Signature signature);
		Signature GetSignature(InstanceID entity);

	private:

		// A Queue is used because it's the most efficient way to remove the first element and add an element at the end
		std::queue<InstanceID> m_AvailableEntities;

		// Array with fixed size to store the signatures of each entity
		std::array<Signature, MAX_ENTITIES> m_AllSignatures;

		// Keep track of the number of living entities
		uint32_t m_LivingEntityCount;
	};

}