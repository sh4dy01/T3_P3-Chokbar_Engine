#pragma once

#include "Engine/IResourceObject.h"

#define SHADER_SIMPLE "Simple"
#define SHADER_TEXTURE "Texture"

class ShaderBase;
class Material;

class Resource
{
public:
	Resource() = default;
	~Resource();

	template <class T = IResourceObject>
	static T* Load(const std::wstring& filepath)
	{
		T* resource = new T(filepath);
		resource->Load();
		return resource;
	}

	static void CreateResources(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize);

	static std::unordered_map<std::string, ShaderBase*>& GetShaders() { return m_shaders; }

private:
	static std::unordered_map<std::string, ShaderBase*> m_shaders;
	static std::unordered_map<std::string, Material*> m_materials;

	static ShaderBase* FindShader(std::string& shaderName);

	static void CreateShaders(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize);

	static void CreateMaterials();


};