#include "Chokbar.h"
#include "ShaderBase.h"

#include "Material.h"

Material::Material()
{
	m_shader = nullptr;
}

Material::~Material()
{
	m_shader = nullptr;
}

void Material::SetShader(ShaderBase * shader)
{
	m_shader = shader;
}
