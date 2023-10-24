#include "Chokbar.h"
#include "D3DUtils.h"
#include <comdef.h>

DxException::DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
    ErrorCode(hr),
    FunctionName(functionName),
    Filename(filename),
    LineNumber(lineNumber)
{
}

std::wstring DxException::ToString()const
{
    // Get the string description of the error code.
    _com_error err(ErrorCode);
    std::wstring msg = err.ErrorMessage();

    return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
}

XMFLOAT4X4 Identity4x4()
{
	static XMFLOAT4X4 I(
		1.0F, 0.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F, 0.0F,
		0.0F, 0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 0.0F, 1.0F);

	return I;
}

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
