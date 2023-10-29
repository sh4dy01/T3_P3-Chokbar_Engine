#include "Chokbar.h"

#include "Core/D3D/Internal/ShaderBase.h"
#include "Core/D3D/Internal/Material.h"

#include "Resource.h"

std::unordered_map<MaterialType, ShaderBase*> Resource::m_shaders;
std::unordered_map<MaterialType, Material*> Resource::m_materials;

Resource::~Resource()
{
	m_shaders.clear();
	m_materials.clear();
}

Material* Resource::LoadMaterial(MaterialType& matType)
{
	return m_materials[matType];
}

void Resource::CreateResources(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize)
{
	CreateShaders(device, cbvHeap, cbvSrvDescriptorSize);
	CreateMaterials();
}

ShaderBase* Resource::FindShader(MaterialType& id)
{
	auto iter = m_shaders.find(id);
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
	std::wstring shaderPathSimple = L"Shader/Simple.hlsl";
	m_shaders[MaterialType::SIMPLE] = new ShaderSimple(device, cbvHeap, cbvSrvDescriptorSize, shaderPathSimple);
	m_shaders[MaterialType::SIMPLE]->Init();

	std::wstring shaderPathTex = L"Shader/Texture.hlsl";
	m_shaders[MaterialType::TEXTURE] = new ShaderTexture(device, cbvHeap, cbvSrvDescriptorSize, shaderPathTex);
	m_shaders[MaterialType::TEXTURE]->Init();

	CreateMaterials();
}

void Resource::CreateMaterials()
{
	m_materials[MaterialType::SIMPLE] = new Material();
	m_materials[MaterialType::SIMPLE]->SetShader(m_shaders[MaterialType::SIMPLE]);

	m_materials[MaterialType::TEXTURE] = new Material();
	m_materials[MaterialType::TEXTURE]->SetShader(m_shaders[MaterialType::TEXTURE]);
}