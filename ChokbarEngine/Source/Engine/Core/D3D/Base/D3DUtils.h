#pragma once

/* ------------------------------------------------------------------------- */
/* HELPER FUNCTIONS                                                          */
/* ------------------------------------------------------------------------- */
#pragma region HelperFunctions

inline DirectX::XMFLOAT4X4 Identity4X4()
{
	static DirectX::XMFLOAT4X4 identity4X4(
		1.0F, 0.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F, 0.0F,
		0.0F, 0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 0.0F, 1.0F
	);

	return identity4X4;
}
#pragma endregion

/* ------------------------------------------------------------------------- */
/* HELPER STRUCTS                                                            */
/* ------------------------------------------------------------------------- */
#pragma region HelperStructs
struct LightData
{
	DirectX::XMFLOAT3 Color = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 Direction = { -1.0f, -1.0f, 0.0f };
};

#pragma endregion