#pragma once

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
		0.0F, 0.0F, 0.0F, 1.0F
	);

	return I;
}

#pragma endregion