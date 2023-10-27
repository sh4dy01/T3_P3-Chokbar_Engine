#include "Chokbar.h"

#include "Core/D3D/Internal/ShaderBase.h"
#include "Core/D3D/Internal/Material.h"

#include "Resource.h"

Resource::~Resource()
{
	m_shaders.clear();
	m_materials.clear();
}

void Resource::CreateResources(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize)
{
	CreateShaders(device, cbvHeap, cbvSrvDescriptorSize);
	CreateMaterials();
}

ShaderBase* Resource::FindShader(std::string& shaderName)
{
	auto iter = m_shaders.find(shaderName);
	if (iter != m_shaders.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

void Resource::CreateShaders(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize)
{
	std::wstring shaderPathSimple = L"Resources/Shaders/Simple.hlsl";
	m_shaders[SHADER_SIMPLE] = new ShaderSimple(device, cbvHeap, cbvSrvDescriptorSize, shaderPathSimple);
	m_shaders[SHADER_SIMPLE]->Init();

	std::wstring shaderPathTex = L"Resources/Shaders/Texture.hlsl";
	m_shaders[SHADER_TEXTURE] = new ShaderTexture(device, cbvHeap, cbvSrvDescriptorSize, shaderPathTex);
	m_shaders[SHADER_TEXTURE]->Init();

	CreateMaterials();
}

void Resource::CreateMaterials()
{
	m_materials[SHADER_SIMPLE] = new Material(SHADER_SIMPLE);
	m_materials[SHADER_SIMPLE]->SetShader(m_shaders[SHADER_SIMPLE]);

	m_materials[SHADER_TEXTURE] = new Material(SHADER_TEXTURE);
	m_materials[SHADER_TEXTURE]->SetShader(m_shaders[SHADER_TEXTURE]);
}