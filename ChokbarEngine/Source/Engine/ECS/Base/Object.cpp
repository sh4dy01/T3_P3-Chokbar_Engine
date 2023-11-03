#include "Chokbar.h"
#include "Object.h"
#include "Engine/Engine.h"

Object::Object()
	: m_Name("DefaultName")
{
	m_InstanceID = Engine::GetCoordinator()->CreateNewObject(this);

}

Object::Object(const std::string& name)
	: m_Name(name)
{
	m_InstanceID = Engine::GetCoordinator()->CreateNewObject(this);
}

Object::~Object()
= default;



void Object::Destroy() const
{
	Engine::GetCoordinator()->DestroyEntity(m_InstanceID);
}
