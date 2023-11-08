#pragma once

class AsteroidLargeBehaviour : public CustomComponent
{
public:
	AsteroidLargeBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(XMFLOAT3 direction, float speed);

private:

	Transform* m_PlayerTransform;

	float m_Speed;

	XMFLOAT3 m_Direction;

};