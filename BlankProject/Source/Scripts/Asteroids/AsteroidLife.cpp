#include "AsteroidLife.h"
#include "AsteroidSpawner.h"

void AsteroidLife::Awake()
{
	LifeComponent::Awake();
}

void AsteroidLife::OnDeath()
{
	AsteroidSpawner::OnDestroyedAsteroid(gameObject);
	gameObject->Destroy();
	//Instantiate particles
}
