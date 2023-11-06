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