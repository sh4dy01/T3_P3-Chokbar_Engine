#include "Chokbar.h"
#include "CameraComponent.h"
#include "Engine/ECS/Components/Component.h"
#include <numbers>

using namespace DirectX;

CameraComponent::CameraComponent()
{
	SetLens(0.25f * std::numbers::pi, 1.0f, 1.0f, 1000.0f);
}

CameraComponent::~CameraComponent()
{
}

XMVECTOR CameraComponent::GetRight() const
{
	return XMLoadFloat3(&m_Right);
}

XMFLOAT3 CameraComponent::GetRight3f() const
{
	return m_Right;
}

XMVECTOR CameraComponent::GetUp() const
{
	return XMLoadFloat3(&m_Up);
}

XMFLOAT3 CameraComponent::GetUp3f() const
{
	return m_Up;
}

XMVECTOR CameraComponent::GetLook() const
{
	return XMLoadFloat3(&m_Look);
}

XMFLOAT3 CameraComponent::GetLook3f() const
{
	return m_Look;
}

float CameraComponent::GetNearZ() const
{
	return m_NearZ;
}

float CameraComponent::GetFarZ() const
{
	return m_FarZ;
}

float CameraComponent::GetAspect() const
{
	return m_Aspect;
}

float CameraComponent::GetFovY() const
{
	return m_FovY;
}

float CameraComponent::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / m_NearZ);
}

float CameraComponent::GetNearWindowWidth() const
{
	return m_Aspect * m_NearWindowHeight;
}

float CameraComponent::GetNearWindowHeight() const
{
	return m_NearWindowHeight;
}

float CameraComponent::GetFarWindowWidth() const
{
	return m_Aspect * m_FarWindowHeight;
}

float CameraComponent::GetFarWindowHeight() const
{
	return m_FarWindowHeight;
}

void CameraComponent::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = zn;
	m_FarZ = zf;

	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

	XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
	XMStoreFloat4x4(&m_Proj, P);
}

void CameraComponent::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMStoreFloat4x4(&m_View, DirectX::XMMatrixLookAtLH(pos, target, worldUp));

	/*
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&transform->GetPosition(), pos);
	XMStoreFloat3(&m_Look, L);
	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
	*/
}

void CameraComponent::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);

	m_ViewDirty = true;
}


XMMATRIX CameraComponent::GetView() const
{
	return XMLoadFloat4x4(&m_View);
}

XMMATRIX CameraComponent::GetProj() const
{
	return XMLoadFloat4x4(&m_Proj);
}

XMFLOAT4X4 CameraComponent::GetView4x4f() const
{
	return m_View;
}

XMFLOAT4X4 CameraComponent::GetProj4x4f() const
{
	return m_Proj;
}

void CameraComponent::UpdateViewMatrix()
{
	if (m_ViewDirty)
	{
		XMVECTOR R = XMLoadFloat3(&m_Right);
		XMVECTOR U = XMLoadFloat3(&m_Up);
		XMVECTOR L = XMLoadFloat3(&m_Look);
		XMVECTOR P = XMLoadFloat3(&transform->GetPosition());
		// Keep camera’s axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));
		// U, L already ortho-normal, so no need to normalize cross product.
			R = XMVector3Cross(U, L);
		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&m_Right, R);
		XMStoreFloat3(&m_Up, U);
		XMStoreFloat3(&m_Look, L);

		m_View(0, 0) = m_Right.x;
		m_View(1, 0) = m_Right.y;
		m_View(2, 0) = m_Right.z;

		m_View(3, 0) = x;
		m_View(0, 1) = m_Up.x;
		m_View(1, 1) = m_Up.y;
		m_View(2, 1) = m_Up.z;

		m_View(3, 1) = y;
		m_View(0, 2) = m_Look.x;
		m_View(1, 2) = m_Look.y;
		m_View(2, 2) = m_Look.z;

		m_View(3, 2) = z;
		m_View(0, 3) = 0.0f;
		m_View(1, 3) = 0.0f;
		m_View(2, 3) = 0.0f;
		m_View(3, 3) = 1.0f;
		m_ViewDirty = false;
	}
}