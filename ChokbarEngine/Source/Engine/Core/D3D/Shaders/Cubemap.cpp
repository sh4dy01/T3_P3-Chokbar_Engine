#include "Chokbar.h"

#include "D3D/Shaders/Texture.h"

#include "Cubemap.h"

Cubemap::Cubemap(const std::string& name) : Texture(name)
{
	m_textType = D3D12_SRV_DIMENSION_TEXTURECUBE;
}

Cubemap::~Cubemap()
= default;
