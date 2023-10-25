#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

class Sphere
{
public:
	Sphere(const XMFLOAT3& center, float radius);

	const XMFLOAT3& GetCenter() const;
	float GetRadius() const;

	void setCenter(const XMFLOAT3& center);
	void setRadius(float radius);

	bool AreSpheresColliding(const Sphere& other) const;

private:
	BoundingSphere m_sphere;
};
