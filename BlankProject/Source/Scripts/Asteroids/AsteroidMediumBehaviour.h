#pragma once
#include "AsteroidBehaviour.h"

class AsteroidMediumBehaviour : public AsteroidBehaviour
{
public:
	AsteroidMediumBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(Transform* target, float speed, const XMFLOAT3& position) override;
};