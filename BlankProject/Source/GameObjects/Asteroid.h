#pragma once


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

    int GetHealth() const;
    void SetHealth(int health);

    void SetDirection(const DirectX::XMFLOAT3& direction);
    DirectX::XMFLOAT3 GetDirection() const;
   
    virtual void Update();
    virtual void TakeDamage(int damage);

protected:
    int m_Health;
    DirectX::XMFLOAT3 m_Direction;
    AsteroidType m_Type;
};