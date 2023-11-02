#pragma once

#include "CollisionShape.h"


class Sphere : public CollisionShape
{
public:
	Sphere(const XMFLOAT3& center, float radius);

	const float GetRadius() { return m_Radius; };

	void SetRadius(float radius);

private:

	float m_Radius;

};
