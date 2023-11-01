#pragma once 

#include <DirectXMath.h>
#include <functional>

using namespace DirectX;

class Rigidbody : public Component
{
public:

    Rigidbody();
    ~Rigidbody() override;

    void RegisterCollisionShape(CollisionShape* shape);
    void RemoveCollisionShape(CollisionShape* shape);

    void RegisterOnTriggerCallback(const std::function<void(CollisionShape*)>& callback);

    void CallOnCollisionEnter(CollisionShape*);


    XMFLOAT3 GetPosition() const;
    void SetPosition(const XMFLOAT3& position);

    XMFLOAT3 GetVelocity() const;
    void SetVelocity(const XMFLOAT3& velocity);

    XMFLOAT3 GetForce() const;
    void AddForce(const XMFLOAT3& force);

    float GetMass() const;
    void SetMass(float mass);

    bool IsStatic() const;
    void SetStatic(bool isStatic);



    std::vector<CollisionShape*> GetAllCollisionShape();

private:

    std::vector<CollisionShape*> m_collisionShapes;
    std::vector<std::function<void(CollisionShape*)>> m_OnTriggerCallback;

    //std::vector<std::function<void(CollisionShape*)>> m_OnCollisionCallback;

    XMFLOAT3 m_position;
    XMFLOAT3 m_velocity;
    XMFLOAT3 m_force;
    float m_mass;

    bool m_isStatic;
};
