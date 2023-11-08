#pragma once

class AsteroidSmallBehaviour : public CustomComponent
{
public:
	AsteroidSmallBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(XMFLOAT3 direction, float speed);

private:

	Transform* m_PlayerTransform;

	float m_Speed;
	float m_LifeTime;

	XMFLOAT3 m_Direction;

};