#pragma once
#include "AsteroidBehaviour.h"

class AsteroidLargeBehaviour : public AsteroidBehaviour
{
public:
	AsteroidLargeBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(XMFLOAT3 direction, float speed, const XMFLOAT3& position) override;
};