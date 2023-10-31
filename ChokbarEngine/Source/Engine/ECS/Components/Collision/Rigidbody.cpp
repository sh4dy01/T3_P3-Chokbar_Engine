#include "Chokbar.h"
#include "Rigidbody.h"

RigidBody::RigidBody()
{
	Engine::GetPhysicsWorld()->RegisterRigidBody(this);
}

RigidBody::~RigidBody()
{
	Engine::GetPhysicsWorld()->RemoveRigidBody(this);

	for (auto& shape : m_collisionShapes)
	{
		delete shape;
		shape = nullptr;
	}
}

void RigidBody::RegisterCollisionShape(CollisionShape* shape)
{
	m_collisionShapes.push_back(shape);
}

void RigidBody::RemoveCollisionShape(CollisionShape* shape)
{
	m_collisionShapes.erase(std::remove(m_collisionShapes.begin(), m_collisionShapes.end(), shape), m_collisionShapes.end());
}

void RigidBody::SetMass(float mass)
{
	m_mass = mass;
}

float RigidBody::GetMass() const
{
	return m_mass;
}

void RigidBody::SetStatic(bool isStatic)
{
	m_isStatic = isStatic;
}

bool RigidBody::IsStatic() const
{
	return m_isStatic;
}

void RigidBody::SetPosition(const XMFLOAT3& position)
{
	m_position = position;
}

XMFLOAT3 RigidBody::GetPosition() const
{
	return m_position;
}

void RigidBody::SetVelocity(const XMFLOAT3& velocity)
{
	m_velocity = velocity;
}

XMFLOAT3 RigidBody::GetVelocity() const
{
	return m_velocity;
}

void RigidBody::AddForce(const XMFLOAT3& force)
{
	m_force.x += force.x;
	m_force.y += force.y;
	m_force.z += force.z;
}

XMFLOAT3 RigidBody::GetForce() const
{
	return m_force;
}

std::vector<CollisionShape*> RigidBody::GetAllCollisionShape()
{
	return m_collisionShapes;
}