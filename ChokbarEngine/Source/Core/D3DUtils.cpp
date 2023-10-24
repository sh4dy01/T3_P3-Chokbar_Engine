#include "Chokbar.h"
#include "D3DUtils.h"

using namespace DirectX;

void Camera::UpdateViewMatrix()
{
    XMVECTOR pos = XMVectorSet(Position.x, Position.y, Position.z, 1.0F);
    XMVECTOR target = XMVectorSet(0.0F, 0.5F, 0.0F, 0.0F);
    XMVECTOR up = XMVectorSet(0.0F, 1.0F, 0.0F, 0.0F);

    XMStoreFloat4x4(&View, XMMatrixLookAtLH(pos, target, up));
}

void Camera::UpdateProjMatrix(const float winWidth, const float winHeight)
{
    XMVECTOR pos = XMVectorSet(Position.x, Position.y, Position.z, 1.0F);
    XMVECTOR target = XMVectorSet(0.0F, 0.5F, 0.0F, 0.0F);
    XMVECTOR up = XMVectorSet(0.0F, 1.0F, 0.0F, 0.0F);

    XMStoreFloat4x4(&Proj, XMMatrixPerspectiveFovLH(XMConvertToRadians(70.0F), winWidth / winHeight, 0.05F, 1000.0F));
}
