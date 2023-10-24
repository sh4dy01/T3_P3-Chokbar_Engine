#pragma once

#include <DirectXMath.h>

/* ------------------------------------------------------------------------- */
/* HELPER FONCTIONS                                                          */
/* ------------------------------------------------------------------------- */
#pragma region HelperFunctions
inline DirectX::XMFLOAT4X4 Identity4x4()
{
	static DirectX::XMFLOAT4X4 I(
		1.0F, 0.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F, 0.0F,
		0.0F, 0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 0.0F, 1.0F);

	return I;
}
#pragma endregion

/* ------------------------------------------------------------------------- */
/* HELPER STRUCTS                                                            */
/* ------------------------------------------------------------------------- */
#pragma region Helper Structs

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 World = Identity4x4();
};

struct PassConstants
{
	DirectX::XMFLOAT4X4 View = Identity4x4();
	DirectX::XMFLOAT4X4 InvView = Identity4x4();
	DirectX::XMFLOAT4X4 Proj = Identity4x4();
	DirectX::XMFLOAT4X4 InvProj = Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = Identity4x4();
	DirectX::XMFLOAT4X4 InvViewProj = Identity4x4();

	DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;
};

struct Camera
{
	DirectX::XMFLOAT3 Position = { 0.0f, 3.0f, -6.0f };
	DirectX::XMFLOAT3 Look = { 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT3 Up = { 0.0f, 1.0f, 0.0f };

	float NearZ = 0.05F;
	float FarZ = 1000.0F;
	float Aspect = 0.0f;

	DirectX::XMFLOAT4X4 View = Identity4x4();
	DirectX::XMFLOAT4X4 Proj = Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = Identity4x4();

	void UpdateViewMatrix();
	void UpdateProjMatrix(const float winWidth, const float winHeight);
};
#pragma endregion