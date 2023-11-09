#pragma once 
#include "Asteroid.h"

class AsteroidMedium : public Asteroid
{
public:
	AsteroidMedium();

	virtual ~AsteroidMedium() override;

	void DestroyAfterATime(float m_Lifetime) override;

private:
	float m_Lifetime;
};