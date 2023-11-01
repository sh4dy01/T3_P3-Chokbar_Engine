#pragma once
#include "ECS/Components/Collision/CollisionShape.h"
#include "ECS/Components/Collision/Rigidbody.h"
#include "ECS/Components/Collision/SphereCollision.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include <list>
#include <vector>
#include <algorithm>

class PhysicsWorld 
{
public:
    PhysicsWorld();
    PhysicsWorld(int gridSize, float cellSize);
    ~PhysicsWorld();

    void Update();

    void RegisterRigidBody(Rigidbody* rigidbody);
    void RemoveRigidBody(Rigidbody* rigidbody);

    bool DetectCollision(Rigidbody* rbA, Rigidbody* rbB) const;

    bool AreSpheresColliding(Sphere* sphere1, Sphere* sphere2) const;
private:
    //struct SpatialGridCell {
    //    std::list<CollisionShape*> objects;
    //};

    std::vector<Rigidbody*> m_rigidbodies;
    int m_gridSize;
    float m_cellSize;
    //SpatialGridCell*** m_grid;

    //SpatialGridCell& getCellForPosition(const XMFLOAT3& position);
    //std::vector<SpatialGridCell*> getAdjacentCells(const XMFLOAT3& position);
};