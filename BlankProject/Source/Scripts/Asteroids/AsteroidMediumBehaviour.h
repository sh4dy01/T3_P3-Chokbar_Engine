#pragma once

class AsteroidMediumBehaviour : public CustomComponent
{
public:
	AsteroidMediumBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(XMFLOAT3 direction, float speed, float lifeTime);

private:

	Transform* m_PlayerTransform;

	float m_Speed;
	float m_LifeTime;

	XMFLOAT3 m_Direction;

};