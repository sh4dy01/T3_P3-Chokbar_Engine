#pragma once 
#include "Engine/ECS/Components/Component.h"
#include "Engine/PhysicsManager.h"

using namespace Chokbar;

class CollisionShape : public Component
{
public:
    CollisionShape();
    virtual bool Intersects(const CollisionShape& other) const = 0;
private:

};