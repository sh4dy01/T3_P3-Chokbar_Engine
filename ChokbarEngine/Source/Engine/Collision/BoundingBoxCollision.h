#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

class BoundingBox
{
public:
	BoundingBox(const XMFLOAT3& center, const XMFLOAT3& extents);

	const XMFLOAT3& GetCenter() const;
	const XMFLOAT3& GetExtents() const;

	void setCenter(const XMFLOAT3& center);
	void setExtents(const XMFLOAT3& extents);

	bool AreBoundingBoxColliding(const BoundingBox& other) const;

private:
	BoundingBox m_box;
};
