#pragma once

#include "System.h"

#include <memory>
#include <unordered_map>
#include <cassert>


namespace Chokbar {

	class SystemManager
	{
	public:

		SystemManager();
		~SystemManager();

	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();

			assert(m_Systems.find(typeName) == m_Systems.end() && "Registering system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			m_Systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();

			assert(m_Systems.find(typeName) != m_Systems.end() && "System used before registered.");

			// Set the signature for this system
			m_Signatures.insert({ typeName, signature });
		}

	public:

		// Erase a destroyed entity from all system lists
		void EntityDestroyed(Entity entity);

		void EntitySignatureChanged(Entity entity, Signature entitySignature);

	private:

		// Map from system type string pointer to a signature
		std::unordered_map<const char*, Signature> m_Signatures;

		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, std::shared_ptr<System>> m_Systems;
	};

}