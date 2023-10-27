#include "Chokbar.h"
#include "TriangleCollision.h"

/// <summary>
/// Constructs a Triangle object with given vertices.
/// </summary>
/// <param name="v1">First vertex of the triangle.</param>
/// <param name="v2">Second vertex of the triangle.</param>
/// <param name="v3">Third vertex of the triangle.</param>
Triangle::Triangle(const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3)
{
    m_vertices[0] = v1;
    m_vertices[1] = v2;
    m_vertices[2] = v3;
}

/// <summary>
/// Checks if the triangle collides with another triangle.
/// </summary>
/// <param name="other">The other triangle to check collision against.</param>
/// <returns>True if the triangles collide, false otherwise.</returns>
bool Triangle::IsCollidingWith(const Triangle& other) const
{
    return CheckTriangleIntersection(*this, other) || CheckTriangleIntersection(other, *this);
}

/// <summary>
/// Uses Möller–Trumbore algorithm to check intersection between two triangles.
/// </summary>
/// <param name="a">First triangle.</param>
/// <param name="b">Second triangle.</param>
/// <returns>True if the triangles intersect, false otherwise.</returns>
bool Triangle::CheckTriangleIntersection(const Triangle& a, const Triangle& b) const
{
    const float EPSILON = 0.000001f;  // Small value to handle floating point inaccuracies

    XMVECTOR e1, e2;  // Edge vectors of triangle a
    XMVECTOR h, s, q;
    float a_val, f, u, v;

    XMVECTOR vert0 = XMLoadFloat3(&a.m_vertices[0]);
    XMVECTOR vert1 = XMLoadFloat3(&a.m_vertices[1]);
    XMVECTOR vert2 = XMLoadFloat3(&a.m_vertices[2]);
    XMVECTOR vertB = XMLoadFloat3(&b.m_vertices[0]);

    // Compute triangle edges
    e1 = vert1 - vert0;
    e2 = vert2 - vert0;

    // Calculate h = ray direction cross e2
    h = XMVector3Cross(vertB - vert0, e2);

    // Calculate a as dot of e1 and h
    a_val = XMVectorGetX(XMVector3Dot(e1, h));

    if (a_val > -EPSILON && a_val < EPSILON) // Triangles are parallel
        return false;

    f = 1.0f / a_val;
    s = vertB - vert0;

    // Compute first barycentric coordinate
    u = f * XMVectorGetX(XMVector3Dot(s, h));
    if (u < 0.0f || u > 1.0f)
        return false;

    q = XMVector3Cross(s, e1);

    // Compute second barycentric coordinate
    v = f * XMVectorGetX(XMVector3Dot(vertB - vert0, q));
    if (v < 0.0f || u + v > 1.0f)
        return false;

    // Compute the intersection point's distance along the ray
    float t = f * XMVectorGetX(XMVector3Dot(e2, q));

    if (t > EPSILON)
        return true;

    return false;
}
