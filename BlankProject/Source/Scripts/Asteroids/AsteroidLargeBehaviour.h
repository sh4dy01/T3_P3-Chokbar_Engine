#pragma once

class AsteroidLargeBehaviour : public CustomComponent
{
public:
	AsteroidLargeBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(XMFLOAT3 direction, float speed, const XMFLOAT3& position);
	void DestroyAfterATime();

	float GetLifetime() const { return m_Lifetime; }


private:

	Transform* m_PlayerTransform;

	float m_Speed;
	float m_Lifetime;
	float m_TimeSinceLastUpdate;
	bool m_PlayerLastPositionSet;

	XMFLOAT3 m_Direction;
	XMFLOAT3 m_Position;
};