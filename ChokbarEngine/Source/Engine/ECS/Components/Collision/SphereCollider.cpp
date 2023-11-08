#include "Chokbar.h"
#include "SphereCollider.h"


SphereCollider::SphereCollider()
	: m_Radius(0.0f)
{
	m_type = ShapeType::Sphere;

	SetRadius(1.0f);
}

SphereCollider::~SphereCollider()
{
}

/// <summary>
/// Sets the radius of the sphere.
/// </summary>
/// <param name="radius">The new radius for the sphere.</param>
void SphereCollider::SetRadius(float radius)
{
	m_Radius = radius;

	SetGridSize(std::ceil(m_Radius * 2 / CELL_SIZE));
}
