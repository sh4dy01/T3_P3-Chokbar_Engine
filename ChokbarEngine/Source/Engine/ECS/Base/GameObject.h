#pragma once
#include "Object.h"
#include "Engine/Engine.h"

namespace Chokbar
{
	class GameObject : public Object
	{
	public:

		GameObject();
		GameObject(const std::string& name);
		~GameObject();

		void AddComponent(Component component) const;

		template<class T>
		T GetComponent()
		{
			return Engine::GetCoordinator().GetComponent<T>(m_InstanceID);
		}

		template<class T>
		bool TryGetComponent(T)
		{
			return Engine::GetCoordinator().HasComponent<T>(m_InstanceID);
		}
		

	public:

		Transform m_Transform;
	};
}


