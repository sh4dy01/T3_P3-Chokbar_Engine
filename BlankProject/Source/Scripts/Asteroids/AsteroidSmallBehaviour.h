#pragma once
#include "AsteroidBehaviour.h"

class AsteroidSmallBehaviour : public AsteroidBehaviour
{
public:
    AsteroidSmallBehaviour() = default;
    void Awake() override;
    void Start() override;
    void Update() override;

    void Initialize(XMFLOAT3 direction, const XMFLOAT3& position);

private:

};