#pragma once
#include "Collider.h"


class SphereCollider : public Collider
{
public:

	SphereCollider();

	const float GetRadius() { return m_Radius; };

	void SetRadius(float radius);

private:

	float m_Radius;

};
