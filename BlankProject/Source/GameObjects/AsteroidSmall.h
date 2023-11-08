#pragma once 
#include "Asteroid.h"

class AsteroidSmall : public Asteroid
{
public:
    AsteroidSmall();

    virtual ~AsteroidSmall() override;

    void TakeDamage(int damage);
};