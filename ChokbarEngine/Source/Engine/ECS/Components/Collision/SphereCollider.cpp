#include "Chokbar.h"
#include "SphereCollider.h"


SphereCollider::SphereCollider()
	: m_Radius(1.0f)
{
	m_type = ShapeType::Sphere;
}

/// <summary>
/// Sets the radius of the sphere.
/// </summary>
/// <param name="radius">The new radius for the sphere.</param>
void SphereCollider::SetRadius(float radius)
{
	m_Radius = radius;
}
