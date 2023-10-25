#include "Chokbar.h"
#include "BoundingBoxCollision.h"

/// <summary>
/// Constructs a BoundingBox object with a given center and extents.
/// </summary>
/// <param name="center">The center point of the BoundingBox.</param>
/// <param name="extents">Half the width, height, and depth of the BoundingBox.</param>
BoundingBox::BoundingBox(const XMFLOAT3& center, const XMFLOAT3& extents) : m_box(center, extents) {}

/// <summary>
/// Gets the center of the BoundingBox.
/// </summary>
/// <returns>The center point of the BoundingBox.</returns>
const XMFLOAT3& BoundingBox::GetCenter() const
{
	return m_box.Center;
}

/// <summary>
/// Gets the extents of the BoundingBox, which represent half its dimensions.
/// </summary>
/// <returns>The extents of the BoundingBox.</returns>
const XMFLOAT3& BoundingBox::GetExtents() const
{
	return m_box.Extents;
}

/// <summary>
/// Sets the center of the BoundingBox.
/// </summary>
/// <param name="center">The new center point for the BoundingBox.</param>
void BoundingBox::setCenter(const XMFLOAT3& center)
{
	m_box.Center = center;
}

/// <summary>
/// Sets the extents of the BoundingBox.
/// </summary>
/// <param name="extents">The new extents for the BoundingBox, representing half its dimensions.</param>
void BoundingBox::setExtents(const XMFLOAT3& extents)
{
	m_box.Extents = extents;
}

/// <summary>
/// Checks if this BoundingBox is colliding with another BoundingBox.
/// </summary>
/// <param name="other">The other BoundingBox to check for collision.</param>
/// <returns>True if the BoundingBox are colliding, false otherwise.</returns>
bool BoundingBox::AreBoundingBoxColliding(const BoundingBox& other) const
{
	return m_box.Intersects(other.m_box);
}
