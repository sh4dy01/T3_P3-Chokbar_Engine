#pragma once
#include "ECS/Components/Collision/Rigidbody.h"
#include "ECS/Components/Collision/SphereCollider.h"

#include <vector>


enum CollisionState {
    Enter,
    Stay,
    Exit
};

class CollisionInfo
{
public:

    CollisionInfo(Collider* colliderA, Collider* colliderB);
    ~CollisionInfo();

    CollisionState GetState() const { return m_State; };
    void UpdateState(CollisionState state);

    Collider* GetColliderA() const { return m_ColliderA; };
    Collider* GetColliderB() const { return m_ColliderB; };



private:

    CollisionState m_State;
    Collider* m_ColliderA;
    Collider* m_ColliderB;

};

class PhysicsWorld 
{
public:

    PhysicsWorld();
    ~PhysicsWorld();

    void RegisterRigidBody(Rigidbody* rigidbody);
    void RemoveRigidBody(Rigidbody* rigidbody);

    void Update(float dt);
   
private:

    void CheckCollision();
    bool CheckCollisionShapes(Rigidbody* rbA, Rigidbody* rbB);
    void HandleCollision(Collider* sphereA, Collider* sphereB);

    bool AreSpheresColliding(SphereCollider* sphere1, SphereCollider* sphere2) const;

    void CreateNewCollisionInfo(Collider* sphereA, Collider* sphereB);
    CollisionInfo* GetCollisionInfo(const Collider* sphereA, const Collider* sphereB) const;


private:

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
