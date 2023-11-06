#include "Chokbar.h"
#include "Rigidbody.h"

using namespace DirectX;

Rigidbody::Rigidbody()
	: m_mass(1.0f), m_isStatic(false), m_velocity(XMFLOAT3(0, 0, 0)), m_force(XMFLOAT3(0, 0, 0))
{
	Engine::GetPhysicsWorld()->RegisterRigidBody(this);
}

Rigidbody::~Rigidbody()
{
	Engine::GetPhysicsWorld()->RemoveRigidBody(this);

	for (auto& shape : m_collisionShapes)
	{
		shape = nullptr;
	}
}

void Rigidbody::RegisterCollisionShape(Collider* shape)
{
	m_collisionShapes.push_back(shape);
}

void Rigidbody::RemoveCollisionShape(Collider* shape)
{
	std::erase(m_collisionShapes, shape);
}

void Rigidbody::SetMass(float mass)
{
	m_mass = mass;
}

float Rigidbody::GetMass() const
{
	return m_mass;
}

void Rigidbody::SetStatic(bool isStatic)
{
	m_isStatic = isStatic;
}

bool Rigidbody::IsStatic() const
{
	return m_isStatic;
}

void Rigidbody::SetVelocity(const XMFLOAT3& velocity)
{
	m_velocity = velocity;
}

XMFLOAT3 Rigidbody::GetVelocity() const
{
	return m_velocity;
}

std::vector<Collider*> Rigidbody::GetAllCollisionShape()
{
	return m_collisionShapes;
}