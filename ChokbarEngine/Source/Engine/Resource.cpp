#include "Chokbar.h"

#include "D3D/Shaders/ShaderBase.h"
#include "D3D/Shaders/Material.h"
#include "IResourceObject.h"

#include "Resource.h"

#include <ranges>

std::unordered_map<MaterialType, ShaderBase*> Resource::m_shaders;
std::unordered_map<MaterialType, Material*> Resource::m_materials;
std::unordered_map<std::string, IResourceObject*> Resource::m_resources;

Resource::~Resource()
= default;

Material* Resource::LoadMaterial(MaterialType matType)
{
	return m_materials[matType];
}

void Resource::CreateResources(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize)
{
	CreateShaders(device, cbvHeap, cbvSrvDescriptorSize);
	CreateMaterials();
}

void Resource::ReleaseResources()
{
	for (auto& mat : m_materials)
	{
		DELPTR(mat.second)
	}

	for (auto& shader : m_shaders)
	{
		DELPTR(shader.second)
	}

	for (auto& resource : m_resources)
	{
		DELPTR(resource.second)
	}


	m_shaders.clear();
	m_materials.clear();
	m_resources.clear();
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

	std::wstring shaderPathTexTrans = L"Shader/Texture_Transparent.hlsl";
	m_shaders[MaterialType::TEXTURE_UI] = new ShaderTextureOffset(device, cbvHeap, cbvSrvDescriptorSize, shaderPathTexTrans);
	m_shaders[MaterialType::TEXTURE_UI]->Init();

	std::wstring shaderPathParticle = L"Shader/Particle.hlsl";
	m_shaders[MaterialType::PARTICLE] = new ShaderParticle(device, cbvHeap, cbvSrvDescriptorSize, shaderPathParticle);
	m_shaders[MaterialType::PARTICLE]->Init();

	std::wstring shaderPathSkybox = L"Shader/Sky.hlsl";
	m_shaders[MaterialType::SKYBOX] = new ShaderSkybox(device, cbvHeap, cbvSrvDescriptorSize, shaderPathSkybox);
	m_shaders[MaterialType::SKYBOX]->Init();
}

void Resource::CreateMaterials()
{
	CreateMaterial(MaterialType::SIMPLE, "Simple");
	CreateMaterial(MaterialType::TEXTURE, "Texture");
	CreateMaterial(MaterialType::TEXTURE_UI, "TextureOffset");
	CreateMaterial(MaterialType::PARTICLE, "Particle");
	CreateMaterial(MaterialType::SKYBOX, "Skybox");
}

void Resource::CreateMaterial(const MaterialType& mt, const std::string& name)
{
	m_materials[mt] = new Material();
	m_materials[mt]->SetShader(m_shaders[mt]);
	m_materials[mt]->Name = name;
}
