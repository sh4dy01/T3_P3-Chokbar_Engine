#include "Chokbar.h"
#include "Rigidbody.h"

Rigidbody::Rigidbody()
	: m_isStatic(false), m_velocity(XMFLOAT3(0, 0, 0)), m_mass(1.f), m_gridPosition(0, 0, 0)
{
	Engine::GetPhysicsWorld()->RegisterRigidBody(this);
}

Rigidbody::Rigidbody(bool isStatic)
	: m_isStatic(isStatic), m_velocity(XMFLOAT3(0, 0, 0)), m_mass(1.f), m_gridPosition(0, 0, 0)
{
	Engine::GetPhysicsWorld()->RegisterRigidBody(this);
}

void Rigidbody::OnAddedComponent()
{
	m_gridPosition = {
		transform->GetPosition().x / CELL_SIZE,
		transform->GetPosition().y / CELL_SIZE,
		transform->GetPosition().z / CELL_SIZE
	};
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

void Rigidbody::Move(float x, float y, float z, Transform::Space space)
{
	transform->Translate(x, y, z, space);

	m_gridPosition = {
		transform->GetPosition().x / CELL_SIZE,
		transform->GetPosition().y / CELL_SIZE,
		transform->GetPosition().z / CELL_SIZE
	};
}

void Rigidbody::Move(const XMFLOAT3& displacement, Transform::Space space)
{
	Move(displacement.x, displacement.y, displacement.z, space);
}

bool Rigidbody::IsStatic() const
{
	return m_isStatic;
}

void Rigidbody::SetVelocity(const XMFLOAT3& velocity)
{
	m_velocity = velocity;
}

void Rigidbody::AddVelocity(const XMFLOAT3& velocity)
{
	m_velocity.x += velocity.x;
	m_velocity.y += velocity.y;
	m_velocity.z += velocity.z;
}

XMFLOAT3 Rigidbody::GetVelocity() const
{
	return m_velocity;
}

std::vector<Collider*> Rigidbody::GetAllCollisionShape()
{
	return m_collisionShapes;
}