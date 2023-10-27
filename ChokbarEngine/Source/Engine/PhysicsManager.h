#pragma once
#include "ECS/Components/Collision/CollisionShape.h"

class PhysicsManager
{
public:
	PhysicsManager();

	void Update(float deltaTime);

	void addCollisionShape(CollisionShape* shape);
	void removeCollisionShape(CollisionShape* shape);

	bool detectCollision(CollisionShape* shape1, CollisionShape* shape2) const;

private:
	std::vector<CollisionShape*> m_CollisionShapes;

public:
	enum class CollisionShapeType
	{
		Box,
		Sphere,
		Pyramid,
		Mesh
	};
};