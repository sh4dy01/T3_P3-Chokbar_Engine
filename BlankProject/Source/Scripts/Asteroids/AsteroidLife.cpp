#include "AsteroidLife.h"
#include "AsteroidSpawner.h"

void AsteroidLife::OnDeath()
{
	AsteroidSpawner::OnDestroyedAsteroid(gameObject);
	gameObject->Destroy();
	//Instantiate particles
}
