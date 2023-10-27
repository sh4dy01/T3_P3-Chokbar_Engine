#pragma once 

class CollisionShape
{
public:
    virtual bool Intersects(const CollisionShape& other) const;
};