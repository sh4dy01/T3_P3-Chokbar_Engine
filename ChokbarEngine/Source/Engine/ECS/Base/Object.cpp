#include "Chokbar.h"
#include "Object.h"
#include "Engine/Engine.h"

Object::Object()
	: m_Name("DefaultName")
{
	m_InstanceID = Coordinator::GetInstance()->GetNewInstanceID();
}

Object::Object(const std::string& name)
	: m_Name(name)
{
	m_InstanceID = Coordinator::GetInstance()->GetNewInstanceID();
}

Object::~Object()
= default;



void Object::Destroy() const
{
	Engine::GetCoordinator()->RegisterDestroyedEntity(m_InstanceID);
}
