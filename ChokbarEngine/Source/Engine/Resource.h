#pragma once

#include <unordered_map>

class ShaderBase;
class Material;
class IResourceObject;
struct ID3D12Device;
struct ID3D12DescriptorHeap;

class Resource
{
public:
	Resource() = default;
	~Resource();

	template <class T = IResourceObject>
	static T* Load(std::string& filepath)
	{
		std::string name = filepath.substr(filepath.find_last_of("/") + 1).substr(0, filepath.find_last_of("."));
		T* resource = NEW T(name);
		resource->Load(filepath);
		m_resources[name] = resource;
		return resource;
	}

	static Material* LoadMaterial(MaterialType matType);

	static void CreateResources(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize);
	static void ReleaseResources();

	static std::unordered_map<MaterialType, ShaderBase*>& GetShaders() { return m_shaders; }
	static std::unordered_map<std::string, IResourceObject*>& GetResources() { return m_resources; }
	static IResourceObject* GetResource(std::string& name) { return m_resources[name]; }

private:
	static std::unordered_map<MaterialType, ShaderBase*> m_shaders;
	static std::unordered_map<MaterialType, Material*> m_materials;
	static std::unordered_map<std::string, IResourceObject*> m_resources;

	static ShaderBase* FindShader(MaterialType& id);

	static void CreateShaders(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvSrvDescriptorSize);

	static void CreateMaterials();
};