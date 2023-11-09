#include "Chokbar.h"

#include "Engine/Engine.h"

#include "CameraComponent.h"

using namespace DirectX;

CameraComponent::CameraComponent()
	: m_ViewDirty(true)
{
	SetLens(70.0F, 1.0f, 1.0f, 5000);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::OnAddedComponent()
{
	if (CameraManager::GetMainCamera() != nullptr) return;

	CameraManager::SetMainCamera(this);
}

XMVECTOR CameraComponent::GetRight() const
{
	return XMLoadFloat3(&transform->GetRight());
}

XMFLOAT3 CameraComponent::GetRight3f() const
{
	return transform->GetRight();
}

XMVECTOR CameraComponent::GetUp() const
{
	return XMLoadFloat3(&transform->GetUp());
}

XMFLOAT3 CameraComponent::GetUp3f() const
{
	return transform->GetUp();
}

XMVECTOR CameraComponent::GetLook() const
{
	return XMLoadFloat3(&transform->GetForward());
}

XMFLOAT3 CameraComponent::GetLook3f() const
{
	if (transform->m_pParent)
	{
		return transform->m_pParent->GetForward();
	}

	return transform->GetForward();
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

void CameraComponent::SetFOV(float fovY)
{
	m_FovY = fovY;

	UpdateWindowWithNewRange();
	UpdateProjectionMatrix();
}

void CameraComponent::SetAspect(float aspect)
{
	m_Aspect = aspect;

	UpdateProjectionMatrix();
}

void CameraComponent::SetZRange(float zn, float zf)
{
	m_NearZ = zn;
	m_FarZ = zf;

	UpdateWindowWithNewRange();
	UpdateProjectionMatrix();
}

void CameraComponent::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = zn;
	m_FarZ = zf;

	UpdateWindowWithNewRange();
	UpdateProjectionMatrix();
}

void CameraComponent::UpdateWindowWithNewRange()
{
	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);
}

void CameraComponent::LookAt(XMFLOAT3 targetPos)
{


	m_ViewDirty = true;
}

XMMATRIX CameraComponent::GetView() const
{
	assert(!m_ViewDirty);

	return XMLoadFloat4x4(&m_View);
}

XMMATRIX CameraComponent::GetOrthoView() const
{
	assert(!m_ViewDirty);

	return XMLoadFloat4x4(&m_OthoView);
}

XMMATRIX CameraComponent::GetProj() const
{
	return XMLoadFloat4x4(&m_Proj);
}

XMMATRIX CameraComponent::GetOrthoProj() const
{
	return XMLoadFloat4x4(&m_OrthoProj);
}

XMMATRIX CameraComponent::GetViewProj() const
{
	return XMMatrixMultiply(GetView(), GetProj());
}

XMMATRIX CameraComponent::GetOrthoViewProj() const
{
	return XMMatrixMultiply(GetOrthoView(), GetOrthoProj());
}

XMFLOAT4X4 CameraComponent::GetView4x4f() const
{
	return m_View;
}

XMFLOAT4X4 CameraComponent::GetProj4x4f() const
{
	return m_Proj;
}

XMFLOAT4X4 CameraComponent::GetViewProj4x4f() const
{
	XMFLOAT4X4 viewProj;
	XMStoreFloat4x4(&viewProj, GetViewProj());
	return viewProj;
}

XMFLOAT4X4 CameraComponent::GetOrthoView4x4f() const
{
	return m_OthoView;
}

XMFLOAT4X4 CameraComponent::GetOrthoProj4x4f() const
{
	return m_OrthoProj;
}

XMFLOAT4X4 CameraComponent::GetOrthoViewProj4x4f() const
{
	XMFLOAT4X4 viewProj;
	XMStoreFloat4x4(&viewProj, GetOrthoViewProj());
	return viewProj;
}

void CameraComponent::UpdateProjectionMatrix()
{
	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FovY), m_Aspect, m_NearZ, m_FarZ));

	float winWidth = Engine::GetWindow()->GetWidth();
	float winHeight = Engine::GetWindow()->GetHeight();
	XMStoreFloat4x4(&m_OrthoProj, XMMatrixOrthographicLH(winWidth, winHeight, m_NearZ, m_FarZ));
}

void CameraComponent::UpdateViewMatrix()
{
	//if (transform->IsDirty() || m_ViewDirty)
	{
		transform->UpdateParentedWorldMatrix();
		XMStoreFloat4x4(&m_View, XMMatrixInverse(&XMMatrixDeterminant(XMLoadFloat4x4(transform->GetParentedWorldMatrix())), XMLoadFloat4x4(transform->GetParentedWorldMatrix())));

		m_ViewDirty = false;

		// Orthographic view
		XMFLOAT3 pos = transform->m_pParent->GetPosition();
		XMFLOAT3 target = transform->m_pParent->GetForward();
		XMStoreFloat4x4(&m_OthoView, XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&transform->GetUp())));
	}
}