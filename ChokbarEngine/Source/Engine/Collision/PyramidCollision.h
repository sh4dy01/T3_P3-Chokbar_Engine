#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "TriangleCollision.h"

class Pyramid
{
public:
    Pyramid(const Triangle& base1, const Triangle& base2, const Triangle& side1, const Triangle& side2, const Triangle& side3, const Triangle& side4);

    bool IsCollidingWith(const Pyramid& other) const;

private:
    Triangle m_triangles[6];
};
