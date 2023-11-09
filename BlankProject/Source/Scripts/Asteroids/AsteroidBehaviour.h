#pragma once

class AsteroidBehaviour : public CustomComponent
{
public:
    AsteroidBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;


	float GetLifetime() const { return m_Lifetime; }

protected: 

	void DestroyAfterATime();

protected:

    float m_Speed;
    float m_Lifetime;
    Rigidbody* m_Rigidbody;

	XMFLOAT3 m_Direction;
	XMFLOAT3 m_Position;

};