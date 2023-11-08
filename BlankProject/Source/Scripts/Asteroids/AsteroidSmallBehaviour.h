#pragma once

class AsteroidSmallBehaviour : public CustomComponent
{
public:
    AsteroidSmallBehaviour() = default;
    void Awake() override;
    void Start() override;
    void Update() override;

    void Initialize(XMFLOAT3 direction, float speed);
    void DestroyAfterATime();

    float GetLifetime() const { return m_Lifetime; }

private:

    Transform* m_PlayerTransform;

    float m_Speed;
    float m_Lifetime;

    XMFLOAT3 m_Direction;

};