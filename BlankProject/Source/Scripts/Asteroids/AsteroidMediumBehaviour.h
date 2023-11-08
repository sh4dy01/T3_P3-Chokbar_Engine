#pragma once

class AsteroidMediumBehaviour : public CustomComponent
{
public:
	AsteroidMediumBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(Transform* target, float speed, const XMFLOAT3& position);
	void DestroyAfterATime();

	float GetLifetime() const { return m_Lifetime; }


private:

	Transform* m_PlayerTransform;

	float m_Speed;
	float m_Lifetime;

	float m_TeleportationAngle;
	float m_TeleportationDistance;

	float m_TimeSinceLastTeleport;
	float m_TeleportInterval;

	XMFLOAT3 m_Position;
};