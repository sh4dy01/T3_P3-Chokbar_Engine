#pragma once

class ProjectileBehavior : public CustomComponent
{
public:

	ProjectileBehavior() = default;

	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(XMFLOAT3 direction, float speed, float lifeTime);

private:

	float m_Speed;
	float m_LifeTime;

	Rigidbody* m_pRigidbody;
	XMFLOAT3 m_Direction;

};