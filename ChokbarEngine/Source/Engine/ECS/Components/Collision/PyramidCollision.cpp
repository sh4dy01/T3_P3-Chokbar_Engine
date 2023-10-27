#include "Chokbar.h"
#include "PyramidCollision.h"

/// <summary>
/// Constructs a Pyramid object with a square base using given triangles.
/// </summary>
/// <param name="base1">First triangle of the square base.</param>
/// <param name="base2">Second triangle of the square base.</param>
/// <param name="side1">First side triangle of the pyramid.</param>
/// <param name="side2">Second side triangle of the pyramid.</param>
/// <param name="side3">Third side triangle of the pyramid.</param>
/// <param name="side4">Fourth side triangle of the pyramid.</param>
Pyramid::Pyramid(const Triangle& base1, const Triangle& base2, const Triangle& side1, const Triangle& side2, const Triangle& side3, const Triangle& side4)
{
    m_triangles[0] = base1;
    m_triangles[1] = base2;
    m_triangles[2] = side1;
    m_triangles[3] = side2;
    m_triangles[4] = side3;
    m_triangles[5] = side4;
}

/// <summary>
/// Checks if the pyramid is colliding with another pyramid.
/// </summary>
/// <param name="other">The other pyramid to check collision against.</param>
/// <returns>True if the pyramids collide, false otherwise.</returns>
bool Pyramid::IsCollidingWith(const Pyramid& other) const
{
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (m_triangles[i].IsCollidingWith(other.m_triangles[j]))
                return true;
        }
    }
    return false;
}

bool Pyramid::Intersects(const CollisionShape& other) const {
    const Pyramid* otherPyramid = dynamic_cast<const Pyramid*>(&other);
    if (otherPyramid) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                if (m_triangles[i].IsCollidingWith(otherPyramid->m_triangles[j])) {
                    return true;
                }
            }
        }
    }
    return false;
}
