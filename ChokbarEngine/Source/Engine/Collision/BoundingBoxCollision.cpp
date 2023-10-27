#include "Chokbar.h"
#include "BoundingBoxCollision.h"

/// <summary>
/// Constructs a Rectangle object with a given center and extents.
/// </summary>
/// <param name="center">The center point of the rectangle.</param>
/// <param name="extents">Half the width, height, and depth of the rectangle.</param>
Rectangle::Rectangle(const XMFLOAT3& center, const XMFLOAT3& extents) : m_box(center, extents) {}

/// <summary>
/// Gets the center of the rectangle.
/// </summary>
/// <returns>The center point of the rectangle.</returns>
const XMFLOAT3& Rectangle::GetCenter() const
{
	return m_box.Center;
}

/// <summary>
/// Gets the extents of the rectangle, which represent half its dimensions.
/// </summary>
/// <returns>The extents of the rectangle.</returns>
const XMFLOAT3& Rectangle::GetExtents() const
{
	return m_box.Extents;
}

/// <summary>
/// Sets the center of the rectangle.
/// </summary>
/// <param name="center">The new center point for the rectangle.</param>
void Rectangle::setCenter(const XMFLOAT3& center)
{
	m_box.Center = center;
}

/// <summary>
/// Sets the extents of the rectangle.
/// </summary>
/// <param name="extents">The new extents for the rectangle, representing half its dimensions.</param>
void Rectangle::setExtents(const XMFLOAT3& extents)
{
	m_box.Extents = extents;
}

/// <summary>
/// Checks if this rectangle is colliding with another rectangle.
/// </summary>
/// <param name="other">The other rectangle to check for collision.</param>
/// <returns>True if the rectangles are colliding, false otherwise.</returns>
bool Rectangle::AreRectanglesColliding(const Rectangle& other) const
{
	return m_box.Intersects(other.m_box);
}

bool Rectangle::Intersects(const CollisionShape& other) const {
    const Rectangle* otherRectangle = dynamic_cast<const Rectangle*>(&other);
    if (otherRectangle) {
        return m_box.Intersects(otherRectangle->m_box);
    }
    return false;
}

