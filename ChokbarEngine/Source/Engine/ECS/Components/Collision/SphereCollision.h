#pragma once

#include "CollisionShape.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

class Sphere : public CollisionShape
{
public:
	Sphere();
	Sphere(const XMFLOAT3& center, float radius);

	const XMFLOAT3& GetCenter() const;
	float GetRadius() const;
	BoundingSphere* GetBoundingSphere() { return &m_sphere; }

	void setCenter(const XMFLOAT3& center);
	void setRadius(float radius);

private:
	BoundingSphere m_sphere;
};
