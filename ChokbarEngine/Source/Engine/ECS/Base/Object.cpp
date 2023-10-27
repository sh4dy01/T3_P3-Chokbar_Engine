#include "Chokbar.h"
#include "Object.h"


namespace Chokbar
{
	Object::Object()
		: m_Name("DefaultName")
	{
		m_InstanceID = Engine::GetCoordinator()->CreateNewGameObjectWithTransform();
	}

	Object::Object(const std::string& name)
		: m_Name(name)
	{
		m_InstanceID = Engine::GetCoordinator()->CreateNewGameObjectWithTransform();
		DEBUG_LOG("Object created with name: " + name);
	}

	Object::~Object()
	= default;


	Object Object::Instantiate(Object original)
	{
		//Clones the object original and returns the clone.

		Object clone = Object();
		clone.m_Name = original.m_Name;

		return clone;
	}

	/*
	Object Object::Instantiate(GameObject original, Transform parent)
	{
		Object clone = Instantiate(original);

		Chokbar::Engine::GetCoordinator().AddComponent<Transform>(clone.m_InstanceID, parent);

		return Object();
	}
	*/

	Object Object::Instantiate(const Object& original, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, const Transform parent)
	{
		Object clone = Instantiate(original);

		const auto transform = Engine::GetCoordinator()->GetComponent<Transform>(clone.m_InstanceID);
		transform->SetPosition(position);
		transform->Rotate(rotation);

		//TODO: assign parent transform


		return clone;
	}

	void Object::Destroy() const
	{
		Engine::GetCoordinator()->DestroyEntity(m_InstanceID);
	}

}
