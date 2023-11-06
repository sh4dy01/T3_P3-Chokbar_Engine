#pragma once
#include "Collider.h"


class SphereCollider : public Collider
{
public:

	SphereCollider();
	~SphereCollider() override;

	float GetRadius() const { return m_Radius; };

	void SetRadius(float radius);

private:

	float m_Radius;

};
