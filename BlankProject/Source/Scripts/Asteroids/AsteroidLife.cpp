#include "AsteroidLife.h"

void AsteroidLife::Awake()
{
	LifeComponent::Awake();
}

void AsteroidLife::OnDeath()
{
	// if (const auto pr = gameObject->GetComponent<ParticleRenderer>())
	// {
	// 	pr->SetParticleCount(5);
	// 	pr->Play();
	// }
	
	//gameObject->Destroy();
}
