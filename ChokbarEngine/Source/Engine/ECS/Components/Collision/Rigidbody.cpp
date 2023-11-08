#include "Chokbar.h"
#include "Rigidbody.h"

Rigidbody::Rigidbody()
	: m_velocity(XMFLOAT3(0, 0, 0)), m_isStatic(false), m_gridPosition(0, 0, 0), m_mass(100.0f), m_force(XMFLOAT3(0, 0, 0)), m_restitution(0.5f)
{
}

Rigidbody::Rigidbody(bool isStatic)
	: m_velocity(XMFLOAT3(0, 0, 0)), m_isStatic(isStatic), m_gridPosition(0, 0, 0), m_mass(100.0f), m_force(XMFLOAT3(0, 0, 0)), m_restitution(0.5f)
{
}

void Rigidbody::OnAddedComponent()
{
	m_gridPosition = {
			static_cast<int>(transform->GetPosition().x) / CELL_SIZE,
			static_cast<int>(transform->GetPosition().y) / CELL_SIZE,
			static_cast<int>(transform->GetPosition().z) / CELL_SIZE
	};
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::SetStatic(bool isStatic)
{
	m_isStatic = isStatic;
}

void Rigidbody::Move(float x, float y, float z, Transform::Space space)
{
	transform->Translate(x, y, z, space);

	m_gridPosition = {
			static_cast<int>(transform->GetPosition().x) / CELL_SIZE,
			static_cast<int>(transform->GetPosition().y) / CELL_SIZE,
			static_cast<int>(transform->GetPosition().z) / CELL_SIZE
	};
}

void Rigidbody::Move(const XMFLOAT3& displacement, Transform::Space space)
{
	Move(displacement.x, displacement.y, displacement.z, space);
}

void Rigidbody::Move(const XMVECTOR& displacement, Transform::Space space)
{
	XMFLOAT3 displacementFloat3;
	XMStoreFloat3(&displacementFloat3, displacement);

	Move(displacementFloat3, space);
}

bool Rigidbody::IsStatic() const
{
	return m_isStatic;
}

void Rigidbody::SetVelocity(const XMFLOAT3& velocity)
{
	m_velocity = velocity;
}

void Rigidbody::SetVelocity(float x, float y, float z)
{
	m_velocity.x = x;
	m_velocity.y = y;
	m_velocity.z = z;
}

void Rigidbody::AddVelocity(float x, float y, float z)
{
	m_velocity.x += x;
	m_velocity.y += y;
	m_velocity.z += z;
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

void Rigidbody::SetMass(float mass)
{
	m_mass = mass;
}

void Rigidbody::SetRestitution(float restitution)
{
	m_restitution = restitution;
}

float Rigidbody::GetRestitution() const
{
	return m_restitution;
}

float Rigidbody::GetMass() const
{
	return m_mass;
}
