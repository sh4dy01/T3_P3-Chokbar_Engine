#pragma once 

#include <DirectXMath.h>

class Collider;

class Rigidbody : public Component
{
public:

    Rigidbody();
    ~Rigidbody() override;

    DirectX::XMFLOAT3 GetVelocity() const;
    void SetVelocity(const DirectX::XMFLOAT3& velocity);

    DirectX::XMFLOAT3 GetForce() const;
    void AddForce(const DirectX::XMFLOAT3& force);

    float GetMass() const;
    void SetMass(float mass);

    bool IsStatic() const;
    void SetStatic(bool isStatic);



private:

    DirectX::XMFLOAT3 m_velocity;
    DirectX::XMFLOAT3 m_force;

    float m_mass;

    bool m_isStatic;
};
