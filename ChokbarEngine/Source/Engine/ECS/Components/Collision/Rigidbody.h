#pragma once 


using namespace DirectX;

class Collider;

class Rigidbody : public Component
{
public:

    Rigidbody(bool isStatic);
    ~Rigidbody() override;

    void RegisterCollisionShape(Collider* shape);
    void RemoveCollisionShape(Collider* shape);

    XMFLOAT3 GetVelocity() const;
    void SetVelocity(const XMFLOAT3& velocity);

    XMFLOAT3 GetForce() const;
    void AddForce(const XMFLOAT3& force);

    float GetMass() const;
    void SetMass(float mass);

    bool IsStatic() const;
    void SetStatic(bool isStatic);



    std::vector<Collider*> GetAllCollisionShape();

private:

    std::vector<Collider*> m_collisionShapes;

    XMFLOAT3 m_velocity;
    XMFLOAT3 m_force;

    float m_mass;

    bool m_isStatic;
};
