#include "Chokbar.h"
#include "Object.h"
#include "Engine/Engine.h"

Object::Object()
	: m_Name("DefaultName")
{
}

Object::Object(const std::string& name)
	: m_Name(name)
{
	m_InstanceID = Chokbar::Engine::GetInstance()->GetCoordinator()->CreateNewGameObjectWithTransform()->GetInstanceID();
}

Object::~Object()
= default;


Object* Object::Instantiate(Object original)
{
	//Clones the object original and returns the clone.

	return new Object(original);
}

/*
Object Object::Instantiate(GameObject original, Transform parent)
{
	Object clone = Instantiate(original);

	Chokbar::Engine::GetCoordinator().AddComponent<Transform>(clone.m_InstanceID, parent);

	return Object();
}
*/

Object* Object::Instantiate(const Object& original, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, const Transform parent)
{
	auto clone = Instantiate(original);

	/*
	const auto transform = Chokbar::Engine::GetCoordinator()->GetComponent<Transform>(clone.m_InstanceID);
	transform->SetPosition(position);
	transform->Rotate(rotation);
	*/
	//TODO: assign parent transform


	return clone;
}

void Object::Destroy() const
{
	Chokbar::Engine::GetCoordinator()->DestroyEntity(m_InstanceID);
}
