#pragma once
#include "Scripts/LifeComponent.h"

class PlanetLife : public LifeComponent
{
protected:

	void Awake() override;
	void OnDeath() override;

};

