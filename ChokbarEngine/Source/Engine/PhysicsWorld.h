#pragma once
#include "ECS/Components/Collision/CollisionShape.h"
#include "ECS/Components/Collision/Rigidbody.h"
#include "ECS/Components/Collision/SphereCollision.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include <list>
#include <vector>
#include <algorithm>


enum CollisionState {
    Enter,
    Stay,
    Exit
};

class CollisionInfo
{
public:

    CollisionInfo(CollisionShape* colliderA, CollisionShape* colliderB);
    ~CollisionInfo();

    CollisionState GetState() const { return m_State; };
    void UpdateState(CollisionState state);

    CollisionShape* GetColliderA() const { return m_ColliderA; };
    CollisionShape* GetColliderB() const { return m_ColliderB; };

private:

    CollisionState m_State;
    CollisionShape* m_ColliderA;
    CollisionShape* m_ColliderB;

};

class PhysicsWorld 
{
public:

    PhysicsWorld();
    PhysicsWorld(int gridSize, float cellSize);
    ~PhysicsWorld();

    void RegisterRigidBody(Rigidbody* rigidbody);
    void RemoveRigidBody(Rigidbody* rigidbody);

    void Update(float dt);
   
private:

    void CheckCollision();
    bool CheckCollisionShapes(Rigidbody* rbA, Rigidbody* rbB);
    void HandleCollision(CollisionShape* sphereA, CollisionShape* sphereB);

    bool AreSpheresColliding(Sphere* sphere1, Sphere* sphere2) const;

    void CreateNewCollisionInfo(CollisionShape* sphereA, CollisionShape* sphereB);
    CollisionInfo* GetCollisionInfo(const CollisionShape* sphereA, const CollisionShape* sphereB) const;


private:

    const float UPDATE_RATE = 0.02f;

    //struct SpatialGridCell {
//    std::list<CollisionShape*> objects;
//};

    std::vector<Rigidbody*> m_rigidbodies;
    std::vector<CollisionInfo*> m_RegisteredCollisionInfos;

    CollisionInfo* m_CurrentCollisionInfo;

    int m_gridSize;
    float m_cellSize;
    float m_timer;



    //SpatialGridCell*** m_grid;

    //SpatialGridCell& getCellForPosition(const XMFLOAT3& position);
    //std::vector<SpatialGridCell*> getAdjacentCells(const XMFLOAT3& position);
};
