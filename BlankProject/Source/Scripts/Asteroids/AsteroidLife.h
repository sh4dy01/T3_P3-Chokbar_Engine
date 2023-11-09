#pragma once
#include "Scripts/LifeComponent.h"

class AsteroidLife : public LifeComponent
{
public:

	void Awake() override;
	
	void OnDeath() override;

};

