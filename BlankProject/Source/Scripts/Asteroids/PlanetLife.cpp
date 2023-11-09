#include "PlanetLife.h"

void PlanetLife::Awake()
{
	InitMaxLife(6);
}

void PlanetLife::OnDeath()
{
	gameObject->Destroy();
}
