#pragma once

/* ------------------------------------------------------------------------- */
/* HELPER FONCTIONS                                                          */
/* ------------------------------------------------------------------------- */
#pragma region HelperFunctions

inline DirectX::XMFLOAT4X4 Identity4x4()
{
	static DirectX::XMFLOAT4X4 Id(
		1.0F, 0.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F, 0.0F,
		0.0F, 0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 0.0F, 1.0F
	);

	return Id;
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