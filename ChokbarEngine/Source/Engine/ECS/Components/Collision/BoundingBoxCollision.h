#pragma once

#include "CollisionShape.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;


class Rectangle : public CollisionShape
{
public:
	Rectangle(const XMFLOAT3& center, const XMFLOAT3& extents);

	const XMFLOAT3& GetCenter() const;
	const XMFLOAT3& GetExtents() const;

	void setCenter(const XMFLOAT3& center);
	void setExtents(const XMFLOAT3& extents);

	bool AreRectanglesColliding(const Rectangle& other) const;

	bool Intersects(const CollisionShape& other) const override;

private:
	BoundingBox m_box;
};
