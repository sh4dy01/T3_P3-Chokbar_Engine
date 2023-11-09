#pragma once 
#include "Asteroid.h"

class AsteroidLarge : public Asteroid {
public:
    AsteroidLarge();

    virtual ~AsteroidLarge() override;

    void DestroyAfterATime(float m_Lifetime) override;

private:
    float m_Lifetime;
};