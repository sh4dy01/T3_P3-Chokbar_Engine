#include "Chokbar.h"
#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
	Engine::GetPhysicsWorld()->RegisterRigidBody(this);
}

Rigidbody::~Rigidbody()
{
	Engine::GetPhysicsWorld()->RemoveRigidBody(this);

	for (auto& shape : m_collisionShapes)
	{
		delete shape;
		shape = nullptr;
	}
}

void Rigidbody::RegisterCollisionShape(CollisionShape* shape)
{
	m_collisionShapes.push_back(shape);
}

void Rigidbody::RemoveCollisionShape(CollisionShape* shape)
{
	m_collisionShapes.erase(std::remove(m_collisionShapes.begin(), m_collisionShapes.end(), shape), m_collisionShapes.end());
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

void Rigidbody::SetPosition(const XMFLOAT3& position)
{
	m_position = position;
}

XMFLOAT3 Rigidbody::GetPosition() const
{
	return m_position;
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

std::vector<CollisionShape*> Rigidbody::GetAllCollisionShape()
{
	return m_collisionShapes;
}