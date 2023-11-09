#pragma once
#include "Scripts/LifeComponent.h"

class AsteroidLife : public LifeComponent
{
public:

	void Awake() override;
	void Start() override;
	
	void OnDeath() override;

private:

	ParticleRenderer* m_ParticleRenderer;

};

