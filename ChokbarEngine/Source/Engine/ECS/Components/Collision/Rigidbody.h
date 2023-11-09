#pragma once

#include <DirectXMath.h>
using namespace DirectX;

#include "Engine/ECS/Components/TransformComponent.h"

enum BodyType
{
	Static,
	// Kinematic,
	Dynamic
};

class Collider;

class Rigidbody : public Component
{
public:
	Rigidbody();
	~Rigidbody() override;

	void OnAddedComponent() override;

	XMFLOAT3 GetVelocity() const;

	void SetVelocity(const XMFLOAT3 &velocity);
	void SetVelocity(float x, float y, float z);
	void AddVelocity(const XMFLOAT3 &velocity);
	void AddVelocity(float x, float y, float z);

	void AddForce(const XMFLOAT3 &force);
	XMFLOAT3 GetForce() const;

	float GetMass() const;
	void SetMass(float mass);

	float GetRestitution() const;
	void SetRestitution(float restitution);

	void Move(float x, float y, float z, Transform::Space space = Transform::Space::Local);
	void Move(const XMFLOAT3 &displacement, Transform::Space space = Transform::Space::Local);
	void Move(const XMVECTOR &displacement, Transform::Space space = Transform::Space::Local);

	void SetBodyType(BodyType bodyType) { m_BodyType = bodyType; }

	BodyType GetBodyType() const { return m_BodyType; }
	XMINT3 GetGridPosition() const { return m_gridPosition; }

private:
	std::vector<Collider *> m_collisionShapes;

	BodyType m_BodyType;

	XMFLOAT3 m_velocity;
	XMFLOAT3 m_force;

	float m_mass;
	float m_restitution;

	XMINT3 m_gridPosition;
};