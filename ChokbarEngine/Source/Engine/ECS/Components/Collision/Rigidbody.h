#pragma once 

#include <DirectXMath.h>
using namespace DirectX;

#include "Engine/ECS/Components/TransformComponent.h"

class Collider;

class Rigidbody : public Component
{
public:

	Rigidbody();
	Rigidbody(bool isStatic);
	~Rigidbody() override;

	void OnAddedComponent() override;
	void RegisterCollisionShape(Collider* shape);
	void RemoveCollisionShape(Collider* shape);

	XMFLOAT3 GetVelocity() const;

	void SetVelocity(const XMFLOAT3& velocity);
	void SetVelocity(float x, float y, float z);
	void AddVelocity(const XMFLOAT3& velocity);
	void AddVelocity(float x, float y, float z);

	void AddForce(const XMFLOAT3& force);
	XMFLOAT3 GetForce() const;

	float GetMass() const;
	void SetMass(float mass);

	float GetRestitution() const;
	void SetRestitution(float restitution);

	bool IsStatic() const;
	void SetStatic(bool isStatic);

	void Move(float x, float y, float z, Transform::Space space = Transform::Space::Local);
	void Move(const XMFLOAT3& displacement, Transform::Space space = Transform::Space::Local);
	void Move(const XMVECTOR& displacement, Transform::Space space = Transform::Space::Local);

	std::vector<Collider*> GetAllCollisionShape();

	const XMINT3 GetGridPosition() const { return m_gridPosition; }

private:
	std::vector<Collider*> m_collisionShapes;

    DirectX::XMFLOAT3 m_velocity;
    DirectX::XMFLOAT3 m_force;

	float m_mass;
	float m_restitution;

	bool m_isStatic;

	XMINT3 m_gridPosition;
};