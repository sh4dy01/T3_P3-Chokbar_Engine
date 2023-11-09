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
	static T* Load(const char* filepath)
	{
		std::string path = filepath;

		std::string name = path.substr(path.find_last_of("/") + 1, path.find_last_of("."));

		if (auto iter = m_resources.find(name) != m_resources.end())
		{
			IResourceObject* resource = m_resources[name];
			T* cRes = reinterpret_cast<T*>(resource);
			return cRes;
		}

		T* resource = new T(name);
		resource->Load(path);
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
	static void CreateMaterial(const MaterialType& mt, const std::string& name);
};