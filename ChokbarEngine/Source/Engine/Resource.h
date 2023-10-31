#pragma once

#include "Engine/IResourceObject.h"
#include "Core/D3D/MaterialType.h"


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

	static Material* LoadMaterial(MaterialType matType);

	static void CreateResources(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize);

	static std::unordered_map<MaterialType, ShaderBase*>& GetShaders() { return m_shaders; }

private:
	static std::unordered_map<MaterialType, ShaderBase*> m_shaders;
	static std::unordered_map<MaterialType, Material*> m_materials;

	static ShaderBase* FindShader(MaterialType& id);

	static void CreateShaders(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize);

	static void CreateMaterials();


};