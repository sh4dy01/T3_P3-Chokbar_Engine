#pragma once
#include "../Scripts/Asteroids/AsteroidLife.h"


class Asteroid : public GameObject 
{
public:

    enum AsteroidType {
        SMALL,
        MEDIUM,
        LARGE,
        NONE
    };

    Asteroid();
    virtual ~Asteroid();

    AsteroidType GetType() const;
    void SetType(AsteroidType type);

    void SetDirection(const DirectX::XMFLOAT3& direction);
    DirectX::XMFLOAT3 GetDirection() const;
   
    virtual void Update();
    virtual void DestroyAfterATime(float m_Lifetime);

protected:

    AsteroidLife* m_Life;

    DirectX::XMFLOAT3 m_Direction;
    AsteroidType m_Type;

    float m_Lifetime;
};