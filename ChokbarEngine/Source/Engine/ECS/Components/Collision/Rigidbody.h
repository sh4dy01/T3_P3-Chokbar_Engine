#pragma once 

using namespace DirectX;

class Collider;

class Rigidbody : public Component
{
public:

    Rigidbody();
    ~Rigidbody() override;

    void RegisterCollisionShape(Collider* shape);
    void RemoveCollisionShape(Collider* shape);

    DirectX::XMFLOAT3 GetVelocity() const;
    void SetVelocity(const DirectX::XMFLOAT3& velocity);

    float GetMass() const;
    void SetMass(float mass);

    bool IsStatic() const;
    void SetStatic(bool isStatic);

    void Move(const XMFLOAT3& displacement);

    std::vector<Collider*> GetAllCollisionShape();

private:
    std::vector<Collider*> m_collisionShapes;

    XMFLOAT3 m_velocity;

    float m_mass;   

    bool m_isStatic;
};