#pragma once
#include "AsteroidBehaviour.h"

class AsteroidLargeBehaviour : public AsteroidBehaviour
{
public:
	AsteroidLargeBehaviour() = default;
	void Awake() override;
	void Start() override;
	void Update() override;

	void Initialize(Transform* target, const XMFLOAT3& position);

private:

	Transform* m_Target;
};