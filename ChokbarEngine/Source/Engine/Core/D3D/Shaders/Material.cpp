#include "Chokbar.h"
#include "ShaderBase.h"

#include "Material.h"

Material::Material() : Name("Material")
{
	m_shader = nullptr;
}

Material::~Material()
{
	// We don't own the shader, so we don't delete it
	m_shader = nullptr;
}

void Material::SetShader(ShaderBase * shader)
{
	m_shader = shader;
}
