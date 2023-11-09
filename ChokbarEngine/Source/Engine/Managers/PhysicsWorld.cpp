#include "Chokbar.h"
#include "PhysicsWorld.h"

#include "Engine/Engine.h"

using namespace DirectX;

CollisionInfo::CollisionInfo(Collider *colliderA, Collider *colliderB)
	: m_ColliderA(colliderA), m_ColliderB(colliderB), m_State(Enter)
{
}

CollisionInfo::~CollisionInfo()
{
	m_ColliderA = nullptr;
	m_ColliderB = nullptr;
}

void CollisionInfo::UpdateState(CollisionState newState)
{
	m_State = newState;
}

PhysicsWorld::PhysicsWorld()
	: m_CurrentCollisionInfo(nullptr), m_gridSize(0), m_cellSize(0.0f), m_timer(0.0f)
{
}

PhysicsWorld::~PhysicsWorld()
{
	CleanUp();
}

void PhysicsWorld::CleanUp() 
{
	for (auto& rigidbody : m_RegisteredCollider)
	for (auto &rigidbody : m_RegisteredCollider)
	{
		rigidbody = nullptr;
	}

	for (auto &collisionInfo : m_RegisteredCollisionInfos)
	{
		DELPTR(collisionInfo);
	}

	m_RegisteredCollider.clear();
	m_RegisteredCollisionInfos.clear();
	m_CurrentCollisionInfo = nullptr;
}

void PhysicsWorld::RegisterCollider(Collider *collider)
{
	m_RegisteredCollider.push_back(collider);
}

void PhysicsWorld::RemoveCollider(Collider *collider)
{
	if (m_RegisteredCollider.empty())
		return;

	std::erase(m_RegisteredCollider, collider);

	for (auto collisionInfo : m_RegisteredCollisionInfos)
	{
		if (collisionInfo->GetColliderA() == collider || collisionInfo->GetColliderB() == collider)
		{
			std::erase(m_RegisteredCollisionInfos, collisionInfo);
			DELPTR(collisionInfo);
		}
	}
}

XMFLOAT3 PhysicsWorld::ReduceVelocity(XMFLOAT3 &velocity)
{
	/*velocity.x *= 0.9f;
	velocity.y *= 0.9f;
	velocity.z *= 0.9f;*/

	return velocity;
}

bool PhysicsWorld::IsVelocityNull(const XMFLOAT3 velocity)
{
	return std::abs(velocity.x) <= FLT_EPSILON &&
		   std::abs(velocity.y) <= FLT_EPSILON &&
		   std::abs(velocity.z) <= FLT_EPSILON;
}

bool PhysicsWorld::IsSameGridPos(const XMINT3 iGridPos, const int iGridSize, XMINT3 jGridPos, int jGridSize)
{
	return std::abs(iGridPos.x - jGridPos.x) < iGridSize + jGridSize &&
		   std::abs(iGridPos.y - jGridPos.y) < iGridSize + jGridSize &&
		   std::abs(iGridPos.z - jGridPos.z) < iGridSize + jGridSize;
}

void PhysicsWorld::Update(float dt)
{
	if (m_timer >= TimeManager::GetFixedTime())
	{
		Coordinator::GetInstance()->FixedUpdateComponents();

		CheckCollision();

		UpdateVelocity(dt);

		m_timer = 0.0f;
	}

	m_timer += dt;
}

void PhysicsWorld::UpdateVelocity(float dt)
{
	for (const auto& collider : m_RegisteredCollider)
	{
		Rigidbody* rb = collider->GetAttachedRigidbody();

		if (rb->GetBodyType() == Static) continue;

		XMFLOAT3 velocity = rb->GetVelocity();

		if (IsVelocityNull(velocity)) continue;

		XMFLOAT3 fixedVelocity = velocity;
		fixedVelocity.x *= dt;
		fixedVelocity.y *= dt;
		fixedVelocity.z *= dt;

		rb->Move(fixedVelocity);

		rb->SetVelocity(ReduceVelocity(velocity));
	}
}

// Placeholder function to convert XMVECTOR to XMFLOAT3
XMFLOAT3 XMVECTORToXMFLOAT3(XMVECTOR v) {
	XMFLOAT3 result;
	XMStoreFloat3(&result, v);
	return result;
}

void PhysicsWorld::CheckCollision()
{
	for (size_t i = 0; i < m_RegisteredCollider.size(); i++)
	{
		for (size_t j = i + 1; j < m_RegisteredCollider.size(); j++)
		{
			if (!m_RegisteredCollider[i]->gameObject->m_CollisionBitmask.IsLayerInMask(m_RegisteredCollider[j]->gameObject->m_CategoryBitmask.GetLayer()) ||
				!m_RegisteredCollider[j]->gameObject->m_CollisionBitmask.IsLayerInMask(m_RegisteredCollider[i]->gameObject->m_CategoryBitmask.GetLayer()))
				continue;

			if (IsSameGridPos(
					m_RegisteredCollider[i]->GetAttachedRigidbody()->GetGridPosition(), m_RegisteredCollider[i]->GetGridSize(),
					m_RegisteredCollider[j]->GetAttachedRigidbody()->GetGridPosition(), m_RegisteredCollider[j]->GetGridSize()) ||
				(m_CurrentCollisionInfo && m_CurrentCollisionInfo->GetState() != Exit))
			{
				
				Collider* colliderA = m_RegisteredCollider[i];
				Collider* colliderB = m_RegisteredCollider[j];

				Rigidbody *rbA = colliderA->GetAttachedRigidbody();
				Transform &aTransform = *colliderA->transform;

				Rigidbody *rbB = colliderB->GetAttachedRigidbody();
				Transform &bTransform = *colliderB->transform;

				if (AreShapesColliding(colliderA, colliderB))
				{
					switch (m_CurrentCollisionInfo->GetState())
					{
					case Enter:
						
						if (m_CurrentCollisionInfo)
							m_CurrentCollisionInfo->GetColliderA()->CallOnTriggerEnter(m_CurrentCollisionInfo->GetColliderB());
						if (m_CurrentCollisionInfo)
							m_CurrentCollisionInfo->GetColliderB()->CallOnTriggerEnter(m_CurrentCollisionInfo->GetColliderA());						

						DEBUG_LOG(m_RegisteredCollider[i]->gameObject->GetName() << " entered in collision with " << m_RegisteredCollider[j]->gameObject->GetName());

						break;
					case Stay:
					{
						// m_rigidbodies[i]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderB);
						// m_rigidbodies[j]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderA);

						//DEBUG_LOG(rbA->gameObject->GetName() << " continue colliding with " << rbB->gameObject->GetName());

						ResolveSphereCollision(rbA, dynamic_cast<SphereCollider*>(colliderA), rbB, dynamic_cast<SphereCollider*>(colliderB));
					}

					break;
					case Exit:

						// m_rigidbodies[i]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderB);
						// m_rigidbodies[j]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderA);

						DEBUG_LOG(m_RegisteredCollider[i]->gameObject->GetName() << " exited collision with " << m_RegisteredCollider[j]->gameObject->GetName())
						std::erase(m_RegisteredCollisionInfos, m_CurrentCollisionInfo);
						DELPTR(m_CurrentCollisionInfo);

						break;
					}

					m_CurrentCollisionInfo = nullptr;
				}
			}
		}
	}
}

bool PhysicsWorld::AreShapesColliding(Collider *shapeA, Collider *shapeB)
{
	if (shapeA->GetType() == Collider::ShapeType::Sphere && shapeB->GetType() == Collider::ShapeType::Sphere)
	{
		const auto sphereA = dynamic_cast<SphereCollider *>(shapeA);
		const auto sphereB = dynamic_cast<SphereCollider *>(shapeB);

		if (AreSpheresColliding(sphereA, sphereB))
		{
			HandleCollision(sphereA, sphereB);

			return true;
		}

		// If there is a collision already registered
		else if (!m_RegisteredCollisionInfos.empty())
		{
			// We check if the collision is still occuring and set it to exit
			if (const auto collisionInfo = GetCollisionInfo(sphereA, sphereB))
			{
				collisionInfo->UpdateState(Exit);
				m_CurrentCollisionInfo = collisionInfo;

				// return true to handle the exit trigger
				return true;
			}
		}
	}

	return false;
}

void PhysicsWorld::HandleCollision(Collider *const sphereA, Collider *const sphereB)
{
	const auto collisionInfo = GetCollisionInfo(sphereA, sphereB);

	if (!collisionInfo)
	{
		CreateNewCollisionInfo(sphereA, sphereB);
	}
	else
	{
		collisionInfo->UpdateState(Stay);
		m_CurrentCollisionInfo = collisionInfo;
	}
}

void PhysicsWorld::CreateNewCollisionInfo(Collider *const sphereA, Collider *const sphereB)
{
	auto newCollisionInfo = NEW CollisionInfo(sphereA, sphereB);

	m_RegisteredCollisionInfos.push_back(newCollisionInfo);
	m_CurrentCollisionInfo = newCollisionInfo;

	newCollisionInfo = nullptr;
}

CollisionInfo *PhysicsWorld::GetCollisionInfo(const Collider *sphereA, const Collider *sphereB) const
{
	for (const auto &collisionInfo : m_RegisteredCollisionInfos)
	{
		if (collisionInfo->GetColliderA() == sphereA && collisionInfo->GetColliderB() == sphereB)
		{
			return collisionInfo;
		}
	}

	return nullptr;
}

bool PhysicsWorld::AreSpheresColliding(SphereCollider *sphereA, SphereCollider *sphereB) const
{
	// Load A's center position and radius
	XMVECTOR vCenterA = XMVectorAdd(XMLoadFloat3(&sphereA->GetCenter()), XMLoadFloat3(&sphereA->transform->GetPosition()));
	float radiusA = sphereA->GetRadius();

	// Load B's center position and radius
	XMVECTOR vCenterB = XMVectorAdd(XMLoadFloat3(&sphereB->GetCenter()), XMLoadFloat3(&sphereB->transform->GetPosition()));
	float radiusB = sphereB->GetRadius();

	// Distance squared between centers
	XMVECTOR delta = XMVectorSubtract(vCenterB, vCenterA);
	XMVECTOR distanceSquared = XMVector3LengthSq(delta);
	float distance = XMVectorGetX(distanceSquared);

	// Sum of the radii squared
	// float radiiSumSquared = (radiusA + radiusB) * (radiusA + radiusB);
	float radiiSumSquared = radiusA * radiusA + radiusB * radiusB;

	// Compare the squared distance to the squared sum of radii
	// return XMVector3LessOrEqual(distanceSquared, XMVectorReplicate(radiiSumSquared));

	return distance <= radiiSumSquared;
}

void PhysicsWorld::ResolveSphereCollision(Rigidbody *rbA, SphereCollider *colliderA, Rigidbody *rbB, SphereCollider *colliderB) const
{
	// Calculate the collision normal
	XMVECTOR posA = XMLoadFloat3(&rbA->transform->GetWorldPosition());
	XMVECTOR posB = XMLoadFloat3(&rbB->transform->GetWorldPosition());
	XMVECTOR collisionNormal = XMVector3Normalize(XMVectorSubtract(posB, posA));

	// Calculate the relative velocity
	XMVECTOR velA = XMLoadFloat3(&rbA->GetVelocity());
	XMVECTOR velB = XMLoadFloat3(&rbB->GetVelocity());
	XMVECTOR relativeVelocity = XMVectorSubtract(velB, velA);

	// Calculate the velocity along the normal
	float velocityAlongNormal = XMVectorGetX(XMVector3Dot(relativeVelocity, collisionNormal));

	// Check if the spheres are already moving apart
	if (velocityAlongNormal > 0) {
		return;
	}

	// Calculate restitution (this should be the smallest restitution of the two colliding bodies)
	float restitution = 0.5f; // assuming perfectly elastic collision for now

	// Calculate impulse scalar
	float impulseScalar = -(1 + restitution) * velocityAlongNormal;
	impulseScalar /= (1 / rbA->GetMass()) + (1 / rbB->GetMass());

	// Apply impulse
	XMVECTOR impulse = XMVectorMultiply(collisionNormal, XMVectorReplicate(impulseScalar));
	XMVECTOR newVelA = XMVectorSubtract(velA, XMVectorMultiply(impulse, XMVectorReplicate(1 / rbA->GetMass())));
	XMVECTOR newVelB = XMVectorAdd(velB, XMVectorMultiply(impulse, XMVectorReplicate(1 / rbB->GetMass())));

	// Convert XMVECTOR to XMFLOAT3
	XMFLOAT3 newVelocityA;
	XMFLOAT3 newVelocityB;
	XMStoreFloat3(&newVelocityA, newVelA);
	XMStoreFloat3(&newVelocityB, newVelB);

	// Set the new velocities
	rbA->AddVelocity(newVelocityA);
	rbB->AddVelocity(newVelocityB);
}
