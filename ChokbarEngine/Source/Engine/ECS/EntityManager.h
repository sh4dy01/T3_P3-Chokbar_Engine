#pragma once

#include <array>
#include <queue>
#include "TypeDef.h"

namespace Chokbar {

	class EntityManager
	{
	public:

		// Initialize the queue with all the entities
		EntityManager(); 
	
		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void SetSignature(Entity entity, Signature signature);
		Signature GetSignature(Entity entity);

	private:

		// A Queue is used because it's the most efficient way to remove the first element and add an element at the end
		std::queue<Entity> m_AvailableEntities;

		// Array with fixed size to store the signatures of each entity
		std::array<Signature, MAX_ENTITIES> m_AllSignatures;

		// Keep track of the number of living entities
		uint32_t m_LivingEntityCoount;
	};

}