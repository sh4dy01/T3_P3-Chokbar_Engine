#include "BlankProject.h"

#include "CrossAir.h"

CrossAir::CrossAir()
{
	auto uir = AddComponent<UIRenderer>();
	uir->Init(SQUARE_UI_SIMPLE, TEXTURE_UI);
	uir->RegisterTexture(Resource::Load<Texture>("Resources/Textures/crossAir1.dds"));
}

CrossAir::~CrossAir()
{
}
