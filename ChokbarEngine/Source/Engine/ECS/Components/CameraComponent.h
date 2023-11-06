#pragma once
#include <DirectXMath.h>

#include "Core/D3D/D3DUtils.h"
#include "Engine/ECS/Components/Component.h"

struct Plane
{
	Plane() = default;
	Plane(const DirectX::XMFLOAT3 point, const DirectX::XMFLOAT3& normal)
		: position(point), normal(normal)
	{
	}

	// unit vector
	XMFLOAT3 normal = { 0.f, 1.f, 0.f };
	XMFLOAT3 position = { 0.f, 0.f, 0.f };

	// distance from origin to the nearest point in the plane
	float     distance = 0.f;
};

struct Frustum
{
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;
};

class CameraComponent : public Component
{
public:
	CameraComponent();
	~CameraComponent() override;

	void OnAddedComponent() override;

	// Get camera basis vectors.
	DirectX::XMVECTOR GetRight() const;
	DirectX::XMFLOAT3 GetRight3f() const;
	DirectX::XMVECTOR GetUp() const;
	DirectX::XMFLOAT3 GetUp3f() const;
	DirectX::XMVECTOR GetLook() const;
	DirectX::XMFLOAT3 GetLook3f() const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetFOV(float fovY);
	void SetAspect(float aspect);
	void SetZRange(float zn, float zf);
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(DirectX::XMFLOAT3 targetPos);

	// Get View/Proj matrices.
	DirectX::XMMATRIX GetView()const;
	DirectX::XMMATRIX GetProj()const;

	DirectX::XMFLOAT4X4 GetView4x4f()const;
	DirectX::XMFLOAT4X4 GetProj4x4f()const;

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();


private:

	void UpdateWindowWithNewRange();
	void UpdateProjectionMatrix();
	void UpdateFrustum();

private:

	const float MIN_Z_NEAR = 0.1f;
	const float MAX_Z_FAR = 1000.f;
	const float BASE_Z_NEAR = 0.5f;
	const float BASE_Z_FAR = 500;

	const DirectX::XMVECTOR m_WorldUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMFLOAT3 m_LookAt = { 0.0f, 0.0f, 1.0f };


	// CameraComponent coordinate system with coordinates relative to world space
	DirectX::XMFLOAT3 m_Right = { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_Up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3 m_Look = { 0.0f, 0.0f, 1.0f };

	// Cache frustum properties.
	float m_NearZ = 0.5f;
	float m_FarZ = 1000.0f;
	float m_Aspect = 0.0f;
	float m_FovY = 75.0f;
	float m_NearWindowHeight = 0.0f;
	float m_FarWindowHeight = 0.0f;

	bool m_ViewDirty;

	Frustum m_Frustum;

	// Cache View/Proj matrices.
	DirectX::XMFLOAT4X4 m_View = Identity4x4();
	DirectX::XMFLOAT4X4 m_Proj = Identity4x4();
};