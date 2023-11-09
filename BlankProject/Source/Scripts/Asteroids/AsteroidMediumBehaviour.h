#pragma once
#include "AsteroidBehaviour.h"

class AsteroidMediumBehaviour : public AsteroidBehaviour
{
public:
	AsteroidMediumBehaviour() = default;

	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(Transform* target, const XMFLOAT3& position);

private:

	Transform* m_Target;

	float m_TeleportationAngle;
	float m_TeleportationDistance;

	float m_TimeSinceLastTeleport;
	float m_TeleportInterval;

	float m_TimeSinceLastUpdate;
	bool m_PlayerLastPositionSet;

};