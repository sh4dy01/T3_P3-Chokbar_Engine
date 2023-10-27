#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

class Triangle
{
public:
    Triangle();
    Triangle(const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3);

    bool IsCollidingWith(const Triangle& other) const;

private:
    XMFLOAT3 m_vertices[3];

    bool CheckTriangleIntersection(const Triangle& a, const Triangle& b) const;
};
