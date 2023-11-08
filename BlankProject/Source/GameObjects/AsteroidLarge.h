#pragma once 
#include "Asteroid.h"

class AsteroidLarge : public Asteroid {
public:
    AsteroidLarge();

    virtual ~AsteroidLarge() override;

    void TakeDamage(int damage);

};