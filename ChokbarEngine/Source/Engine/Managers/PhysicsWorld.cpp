#include "Chokbar.h"
#include "PhysicsWorld.h"

#include "Engine/Engine.h"

using namespace DirectX;

CollisionInfo::CollisionInfo(Collider* colliderA, Collider* colliderB)
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
	for (auto& rigidbody : m_RegisteredCollider)
	{
		rigidbody = nullptr;
	}

	for (auto& collisionInfo : m_RegisteredCollisionInfos)
	{
		DELPTR(collisionInfo);
	}

	m_RegisteredCollider.clear();
	m_RegisteredCollisionInfos.clear();
}

void PhysicsWorld::RegisterCollider(Collider* collider)
{
	m_RegisteredCollider.push_back(collider);
}

void PhysicsWorld::RemoveCollider(Collider* collider)
{
	if (m_RegisteredCollider.empty()) return;

	std::erase(m_RegisteredCollider, collider);
}

XMFLOAT3 PhysicsWorld::ReduceVelocity(XMFLOAT3& velocity)
{
	/*velocity.x *= 0.9f;
	velocity.y *= 0.9f;
	velocity.z *= 0.9f;*/

	return velocity;
}

void PhysicsWorld::Update(float dt)
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

	m_timer += dt;

	if (m_timer >= TimeManager::GetFixedTime())
	{
		Coordinator::GetInstance()->FixedUpdateComponents();

		CheckCollision();

		m_timer = 0.0f;
	}
}

bool PhysicsWorld::IsVelocityNull(const XMFLOAT3 velocity)
{
	return	std::abs(velocity.x) <= FLT_EPSILON &&
		std::abs(velocity.y) <= FLT_EPSILON &&
		std::abs(velocity.z) <= FLT_EPSILON;
}

bool PhysicsWorld::IsSameGridPos(const XMINT3 iGridPos, const int iGridSize, XMINT3 jGridPos, int jGridSize)
{
	return	std::abs(iGridPos.x - jGridPos.x) < iGridSize + jGridSize &&
		std::abs(iGridPos.y - jGridPos.y) < iGridSize + jGridSize &&
		std::abs(iGridPos.z - jGridPos.z) < iGridSize + jGridSize;
}

void PhysicsWorld::CheckCollision()
{
	for (size_t i = 0; i < m_RegisteredCollider.size(); i++)
	{
		for (size_t j = i + 1; j < m_RegisteredCollider.size(); j++)
		{
			if (!m_RegisteredCollider[i]->gameObject->m_CollisionBitmask.IsLayerInMask(m_RegisteredCollider[j]->gameObject->m_CategoryBitmask.GetLayer()) ||
				!m_RegisteredCollider[j]->gameObject->m_CollisionBitmask.IsLayerInMask(m_RegisteredCollider[i]->gameObject->m_CategoryBitmask.GetLayer())) continue;

			if (IsSameGridPos(
				m_RegisteredCollider[i]->GetAttachedRigidbody()->GetGridPosition(), m_RegisteredCollider[i]->GetGridSize(),
				m_RegisteredCollider[j]->GetAttachedRigidbody()->GetGridPosition(), m_RegisteredCollider[j]->GetGridSize()) ||
				(m_CurrentCollisionInfo && m_CurrentCollisionInfo->GetState() != Exit))
			{
				Collider* colliderA = m_RegisteredCollider[i];
				Collider* colliderB = m_RegisteredCollider[j];

				Rigidbody* rbA = colliderA->GetAttachedRigidbody();
				Transform& aTransform = *colliderA->transform;

				Rigidbody* rbB = colliderB->GetAttachedRigidbody();
				Transform& bTransform = *colliderB->transform;

				if (AreShapesColliding(colliderA, colliderB))
				{
					float distance;

					switch (m_CurrentCollisionInfo->GetState())
					{
					case Enter:

						m_CurrentCollisionInfo->GetColliderA()->CallOnTriggerEnter(m_CurrentCollisionInfo->GetColliderB());
						m_CurrentCollisionInfo->GetColliderB()->CallOnTriggerEnter(m_CurrentCollisionInfo->GetColliderA());

						//distance = sqrt(pow(iGridPos.x - jGridPos.x, 2) + pow(iGridPos.y - jGridPos.y, 2) + pow(iGridPos.z - jGridPos.z, 2)) - ((SphereCollider*)m_RegisteredCollider[i])->GetRadius() + ((SphereCollider*)m_RegisteredCollider[j])->GetRadius();


						DEBUG_LOG(m_RegisteredCollider[i]->gameObject->GetName() << " entered in collision with " << m_RegisteredCollider[j]->gameObject->GetName());

						break;
					case Stay:
					{
						//m_rigidbodies[i]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderA);

						DEBUG_LOG(rbA->gameObject->GetName() << " continue colliding with " << rbB->gameObject->GetName());

						//ResolveSphereCollision(rbA, dynamic_cast<SphereCollider*>(colliderA), rbB, dynamic_cast<SphereCollider*>(colliderB));



						//// Calculate the collision normal
						//XMVECTOR collisionNormal = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&bTransform.GetPosition()), XMLoadFloat3(&aTransform.GetPosition())));

						//// Calculate relative velocity
						//XMVECTOR velocityA = XMLoadFloat3(&rbA->GetVelocity());
						//XMVECTOR velocityB = XMLoadFloat3(&rbB->GetVelocity());
						//XMVECTOR relativeVelocity = XMVectorSubtract(velocityA, velocityB);

						//// Calculate the velocity along the normal
						//float velocityAlongNormal = XMVector3Dot(relativeVelocity, collisionNormal).m128_f32[0];

						//// Calculate the restitution (assuming perfect elastic collision for simplicity)
						//float restitution = 1.0f;

						//// Calculate the impulse scalar
						//float massA = rbA->GetMass();
						//float massB = rbB->GetMass();
						//float impulseScalar = -(1 + restitution) * velocityAlongNormal / (massA + massB);

						//// Apply the impulse to each velocity
						//XMVECTOR impulse = XMVectorScale(collisionNormal, impulseScalar);
						//XMVECTOR newVelocityA = XMVectorAdd(velocityA, XMVectorScale(impulse, massB));
						//XMVECTOR newVelocityB = XMVectorSubtract(velocityB, XMVectorScale(impulse, massA));

						//// Convert XMVECTOR velocities back to XMFLOAT3 and set them
						//XMFLOAT3 finalVelocityA, finalVelocityB;
						//XMStoreFloat3(&finalVelocityA, newVelocityA);
						//XMStoreFloat3(&finalVelocityB, newVelocityB);

						//rbA->SetVelocity(finalVelocityA);
						//rbB->SetVelocity(finalVelocityB);



						if (rbA->GetBodyType() == Static && rbB->GetBodyType() == Static) continue;


						// Calculate the distance between the centers of the two spheres
						float distance = sqrt(
							pow(aTransform.GetPosition().x - bTransform.GetPosition().x, 2) +
							pow(aTransform.GetPosition().y - bTransform.GetPosition().y, 2) +
							pow(aTransform.GetPosition().z - bTransform.GetPosition().z, 2)
						);

						// Subtract the sum of the radii to get the actual overlap distance
						distance -= (dynamic_cast<SphereCollider*>(colliderA)->GetRadius() +
							dynamic_cast<SphereCollider*>(colliderB)->GetRadius());

						// Calculate the normalized direction vector from B to A
						XMFLOAT3 directionVec = XMFLOAT3(
							(aTransform.GetPosition().x - bTransform.GetPosition().x) / 10,
							(aTransform.GetPosition().y - bTransform.GetPosition().y) / 10,
							(aTransform.GetPosition().z - bTransform.GetPosition().z) / 10
						);
						XMVECTOR direction = XMVector3Normalize(XMLoadFloat3(&directionVec));

						// Set the velocities based on the direction vector
						// This would need to be calculated based on the collision response (e.g., reflect the velocities)
						// Here is a placeholder for setting the new velocity of rbA; you would need to calculate this properly
						if (rbA->GetBodyType() == Dynamic)
							rbA->SetVelocity(directionVec); // This is just an example and not the correct collision response

						if (rbB->GetBodyType() != Dynamic) continue;

						// The velocity for rbB would be in the opposite direction, but again, you need to calculate it based on physics
						XMFLOAT3 newVelocityB = {
							-directionVec.x,
							-directionVec.y,
							-directionVec.z
						};
						rbB->SetVelocity(newVelocityB);

					}

					break;
					case Exit:

						//m_rigidbodies[i]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderA);

						DEBUG_LOG(m_RegisteredCollider[i]->gameObject->GetName() << " exited collision with " << m_RegisteredCollider[j]->gameObject->GetName())
							std::erase(m_RegisteredCollisionInfos, m_CurrentCollisionInfo);
						DELPTR(m_CurrentCollisionInfo);

						break;
					}
				}
			}

		}

		//DEBUG_LOG(std::to_string(temp));
	}
}

bool PhysicsWorld::AreShapesColliding(Collider* shapeA, Collider* shapeB)
{
	if (shapeA->GetType() == Collider::ShapeType::Sphere && shapeB->GetType() == Collider::ShapeType::Sphere)
	{
		const auto sphereA = dynamic_cast<SphereCollider*>(shapeA);
		const auto sphereB = dynamic_cast<SphereCollider*>(shapeB);

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

void PhysicsWorld::HandleCollision(Collider* const sphereA, Collider* const sphereB)
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


void PhysicsWorld::CreateNewCollisionInfo(Collider* const sphereA, Collider* const sphereB)
{
	auto newCollisionInfo = NEW CollisionInfo(sphereA, sphereB);

	m_RegisteredCollisionInfos.push_back(newCollisionInfo);
	m_CurrentCollisionInfo = newCollisionInfo;

	newCollisionInfo = nullptr;
}

CollisionInfo* PhysicsWorld::GetCollisionInfo(const Collider* sphereA, const Collider* sphereB) const
{
	for (const auto& collisionInfo : m_RegisteredCollisionInfos)
	{
		if (collisionInfo->GetColliderA() == sphereA && collisionInfo->GetColliderB() == sphereB)
		{
			return collisionInfo;
		}
	}

	return nullptr;
}

bool PhysicsWorld::AreSpheresColliding(SphereCollider* sphereA, SphereCollider* sphereB) const
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
	//float radiiSumSquared = (radiusA + radiusB) * (radiusA + radiusB);
	float radiiSumSquared = radiusA * radiusA + radiusB * radiusB;

	// Compare the squared distance to the squared sum of radii
	//return XMVector3LessOrEqual(distanceSquared, XMVectorReplicate(radiiSumSquared));

	return distance <= radiiSumSquared;
}

void PhysicsWorld::ResolveSphereCollision(Rigidbody* rbA, SphereCollider* colliderA, Rigidbody* rbB, SphereCollider* colliderB) const {
	// Calculate the collision normal
	XMVECTOR posA = XMLoadFloat3(&rbA->transform->GetPosition());
	XMVECTOR posB = XMLoadFloat3(&rbB->transform->GetPosition());
	XMVECTOR collisionNormal = XMVector3Normalize(XMVectorSubtract(posB, posA));

	// Calculate relative velocity
	XMVECTOR velocityA = XMLoadFloat3(&rbA->GetVelocity());
	XMVECTOR velocityB = XMLoadFloat3(&rbB->GetVelocity());
	XMVECTOR relativeVelocity = XMVectorSubtract(velocityB, velocityA);
	float velocityAlongNormal = XMVector3Dot(relativeVelocity, collisionNormal).m128_f32[0];

	// Early out if velocities are separating
	if (velocityAlongNormal > 0) {
		return;
	}

	// Restitution (assume some average restitution for now)
	float restitution = 0.5f * (rbA->GetRestitution() + rbB->GetRestitution());

	// Calculate impulse scalar
	float massA = rbA->GetMass();
	float massB = rbB->GetMass();
	float impulseScalar = -(1 + restitution) * velocityAlongNormal / (1 / massA + 1 / massB);

	// Apply impulse
	XMVECTOR impulse = XMVectorScale(collisionNormal, impulseScalar);
	XMVECTOR newVelocityA = XMVectorSubtract(velocityA, XMVectorScale(impulse, 1 / massA));
	XMVECTOR newVelocityB = XMVectorAdd(velocityB, XMVectorScale(impulse, 1 / massB));

	// Update velocities
	XMFLOAT3 newVelA;
	XMStoreFloat3(&newVelA, newVelocityA);
	rbA->SetVelocity(newVelA); // Changed from AddVelocity to SetVelocity

	XMFLOAT3 newVelB;
	XMStoreFloat3(&newVelB, newVelocityB);
	rbB->SetVelocity(newVelB); // Changed from AddVelocity to SetVelocity

	//// Positional Correction
	//float penetrationDepth = colliderA->GetRadius() + colliderB->GetRadius() - XMVector3Length(XMVectorSubtract(posB, posA)).m128_f32[0];
	//if (penetrationDepth > 0.0f) {
	//	const float percent = 0.8f; // typically 80% to 20% of penetration is corrected
	//	const float slop = 0.01f; // small tolerance to avoid jitter
	//	float correction = std::max<float>(penetrationDepth - slop, 0.0f) / (1 / massA + 1 / massB) * percent;

	//	// Calculate correction vector
	//	XMVECTOR correctionVector = XMVectorScale(collisionNormal, correction);

	//	// Apply correction vectors to the positions
	//	XMVECTOR posCorrectionA = XMVectorScale(correctionVector, -1 / massA);
	//	XMVECTOR posCorrectionB = XMVectorScale(correctionVector, 1 / massB);

	//	// Apply the position corrections
	//	posA = XMVectorSubtract(posA, posCorrectionA);
	//	posB = XMVectorAdd(posB, posCorrectionB);

	//	// Set the new positions to the rigid bodies
	//	rbA->Move(posA);
	//	rbB->Move(posB);
	//}
}
