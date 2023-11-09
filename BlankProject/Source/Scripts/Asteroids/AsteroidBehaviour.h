#pragma once

class AsteroidBehaviour : public CustomComponent
{
public:
    AsteroidBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	virtual void Initialize(XMFLOAT3 direction, float speed, const XMFLOAT3& position);
	virtual void Initialize(Transform* target, float speed, const XMFLOAT3& position);

	void DestroyAfterATime();

	float GetLifetime() const { return m_Lifetime; }

protected: 
	Transform* m_PlayerTransform;

    float m_Speed;
    float m_Lifetime;
    Rigidbody* m_Rigidbody;

	XMFLOAT3 m_Direction;
	XMFLOAT3 m_Position;

	float m_TeleportationAngle;
	float m_TeleportationDistance;

	float m_TimeSinceLastTeleport;
	float m_TeleportInterval;

	float m_TimeSinceLastUpdate;
	bool m_PlayerLastPositionSet;
};