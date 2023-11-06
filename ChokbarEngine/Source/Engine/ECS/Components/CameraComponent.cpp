#include "Chokbar.h"
#include "CameraComponent.h"
#include "Engine/ECS/Components/Component.h"
#include <numbers>

using namespace DirectX;

CameraComponent::CameraComponent()
	: m_ViewDirty(true)
{
	SetLens(70.0F, 1.0f, BASE_Z_NEAR, BASE_Z_FAR);
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
	if (zn < MIN_Z_NEAR) zn = MIN_Z_NEAR;
	if (zf > MAX_Z_FAR) zf = MAX_Z_FAR;
	else if (zf <= MIN_Z_NEAR) zf = MIN_Z_NEAR + 0.1f;

	if (zn >= zf) zn = zf - .1f;
	else if (zf <= zn) zf = zn + .1f;

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
	m_LookAt = targetPos;

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
	UpdateFrustum();
}

void CameraComponent::UpdateViewMatrix()
{
	if (transform->IsDirty() || m_ViewDirty)
	{

		XMFLOAT3 Position = transform->GetPosition();
		XMVECTOR pos = XMVectorSet(Position.x, Position.y, Position.z, 1.0F);
		//XMVECTOR target = XMVectorSet(0.0F, 0.5F, 0.0F, 0.0F);
		//XMVECTOR target = XMVectorMultiply(XMLoadFloat3(&Position), XMLoadFloat3(&m_Look));
		XMFLOAT3 forward = transform->GetForward();
		XMVECTOR target = XMVectorAdd(XMLoadFloat3(&Position), XMLoadFloat3(&forward));

		XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(pos, target, XMLoadFloat3(&transform->GetUp())));
		m_ViewDirty = false;
	}
}


void CameraComponent::UpdateFrustum() 
{
	const float halfVSide = m_FarZ * tanf(m_FovY * .5f);
	const float halfHSide = halfVSide * m_Aspect;

	XMFLOAT3 frontMultFar;
	DirectX::XMStoreFloat3(&frontMultFar, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_Look), m_FarZ));

	XMFLOAT3 camPos = transform->GetPosition();

	XMFLOAT3 scaledLookWithFar;
	DirectX::XMStoreFloat3(&scaledLookWithFar, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_Look), m_NearZ));
	m_Frustum.nearFace = Plane(scaledLookWithFar, m_Look);

	XMFLOAT3 camAndFront;
	DirectX::XMStoreFloat3(&camAndFront, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&camPos), DirectX::XMLoadFloat3(&frontMultFar)));

	XMFLOAT3 farFace;
	XMStoreFloat3(&farFace, XMVectorNegate(XMLoadFloat3(&m_Look)));
	m_Frustum.farFace = Plane(camAndFront, farFace);

	XMFLOAT3 rightFace;
	DirectX::XMStoreFloat3(&rightFace, DirectX::XMVector2Cross(DirectX::XMVectorScale(DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&frontMultFar), XMVectorNegate(DirectX::XMLoadFloat3(&m_Right))), halfHSide), XMLoadFloat3(&m_Up)));
	m_Frustum.rightFace = Plane(camPos, rightFace);
	
	XMFLOAT3 leftFace;
	DirectX::XMStoreFloat3(&leftFace, DirectX::XMVector2Cross(XMLoadFloat3(&m_Up), DirectX::XMVectorAdd(DirectX::XMVectorScale(XMLoadFloat3(&m_Right), halfHSide), DirectX::XMLoadFloat3(&frontMultFar))));
	m_Frustum.leftFace = Plane(camPos, leftFace);

	XMFLOAT3 topFace;
	DirectX::XMStoreFloat3(&topFace, DirectX::XMVector2Cross(XMLoadFloat3(&m_Right), DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&frontMultFar), DirectX::XMVectorScale(XMLoadFloat3(&m_Up), halfVSide))));
	m_Frustum.topFace = Plane(camPos, topFace);

	XMFLOAT3 bottomFace;
	DirectX::XMStoreFloat3(&bottomFace, DirectX::XMVector2Cross(XMVectorScale(DirectX::XMLoadFloat3(&m_Up), halfVSide), XMLoadFloat3(&m_Right)));
	m_Frustum.bottomFace = Plane(camPos, bottomFace);
}
