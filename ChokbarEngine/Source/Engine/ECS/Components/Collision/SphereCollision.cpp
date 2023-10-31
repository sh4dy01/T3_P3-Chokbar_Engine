#include "Chokbar.h"
#include "SphereCollision.h"


Sphere::Sphere()
	: m_sphere()
{
	m_type = ShapeType::Sphere;
}
/// <summary>
/// Constructs a Sphere object with a specified center and radius.
/// </summary>
/// <param name="center">The center point of the sphere.</param>
/// <param name="radius">The radius of the sphere.</param>
Sphere::Sphere(const XMFLOAT3& center, float radius)
	: m_sphere(center, radius)
{
	m_type = ShapeType::Sphere;
}

/// <summary>
/// Gets the center of the sphere.
/// </summary>
/// <returns>The center point of the sphere.</returns>
const XMFLOAT3& Sphere::GetCenter() const
{
	return m_sphere.Center;
}

/// <summary>
/// Gets the radius of the sphere.
/// </summary>
/// <returns>The radius of the sphere.</returns>
float Sphere::GetRadius() const
{
	return m_sphere.Radius;
}

/// <summary>
/// Sets the center of the sphere.
/// </summary>
/// <param name="center">The new center point for the sphere.</param>
void Sphere::setCenter(const XMFLOAT3& center)
{
	m_sphere.Center = center;
}

/// <summary>
/// Sets the radius of the sphere.
/// </summary>
/// <param name="radius">The new radius for the sphere.</param>
void Sphere::setRadius(float radius)
{
	m_sphere.Radius = radius;
}
