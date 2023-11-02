#include "Chokbar.h"
#include "SphereCollider.h"


/// <summary>
/// Constructs a Sphere object with a specified center and radius.
/// </summary>
/// <param name="center">The center point of the sphere.</param>
/// <param name="radius">The radius of the sphere.</param>
SphereCollider::SphereCollider(const XMFLOAT3& center, float radius)
{
	m_Center = center;
	m_Radius = radius;
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
