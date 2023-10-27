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
/*

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

*/

#pragma endregion