#include "Chokbar.h"
#include "Rigidbody.h"

Rigidbody::Rigidbody(bool isStatic)
: m_isStatic(isStatic), m_velocity(XMFLOAT3(0, 0, 0))
{
	Chokbar::Engine::GetPhysicsWorld()->RegisterRigidBody(this);
}

Rigidbody::~Rigidbody()
{
	Chokbar::Engine::GetPhysicsWorld()->RemoveRigidBody(this);

	for (auto& shape : m_collisionShapes)
	{
		delete shape;
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

void Rigidbody::AddForce(const XMFLOAT3& force)
{
	m_force.x += force.x;
	m_force.y += force.y;
	m_force.z += force.z;
}

XMFLOAT3 Rigidbody::GetForce() const
{
	return m_force;
}

std::vector<Collider*> Rigidbody::GetAllCollisionShape()
{
	return m_collisionShapes;
}
