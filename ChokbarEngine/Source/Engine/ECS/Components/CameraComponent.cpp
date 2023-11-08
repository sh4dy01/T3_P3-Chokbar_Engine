#include "Chokbar.h"
#include "CameraComponent.h"

using namespace DirectX;

CameraComponent::CameraComponent()
	: m_ViewDirty(true)
{
	SetLens(70.0F, 1.0f, 1.0f, 1000.0f);
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

void CameraComponent::UpdateProjectionMatrix()
{
	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FovY), m_Aspect, m_NearZ, m_FarZ));
}

void CameraComponent::UpdateViewMatrix()
{
	//if (transform->IsDirty() || m_ViewDirty)
	{
		transform->UpdateParentedWorldMatrix();

		auto parentedMAtrix = transform->GetParentedWorldMatrix();
		XMFLOAT3 Position;

		//transform->UpdateParentedWorldMatrix();
		XMVECTOR pos = XMVectorSet(parentedMAtrix->_41, parentedMAtrix->_42, parentedMAtrix->_43, 1.0F);
		XMStoreFloat3(&Position, pos);

		XMFLOAT3 forward = transform->GetForward();
		XMVECTOR target = XMVectorAdd(XMLoadFloat3(&Position), XMLoadFloat3(&forward));

		XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(pos, target, XMLoadFloat3(&transform->GetUp())));
		m_ViewDirty = false;
	}
}