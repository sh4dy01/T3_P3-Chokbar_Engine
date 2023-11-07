#include "Chokbar.h"
#include "Rigidbody.h"

Rigidbody::Rigidbody()
	: m_isStatic(false), m_velocity(XMFLOAT3(0, 0, 0)), m_gridPosition(0, 0, 0)
{
}

Rigidbody::Rigidbody(bool isStatic)
	: m_isStatic(isStatic), m_velocity(XMFLOAT3(0, 0, 0)), m_gridPosition(0, 0, 0)
{
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