#pragma once 

class Material;
class D3DMesh;
class Texture;

#include "Core/D3D/MeshType.h"
#include "Core/D3D/MaterialType.h"

class MeshRenderer : public Component
{
public:

	MeshRenderer();
	MeshRenderer(MeshType, MaterialType);
	~MeshRenderer();

	void OnDelete();

	void BindMaterial(Material* mat);

	Texture* GetTexture(UINT index) const { return m_textures[index]; }
	std::vector<Texture*> GetTextures() const { return m_textures; }
	void RegisterTexture(Texture* tex) { m_textures.push_back(tex); }


public:
	D3DMesh* Mesh;
	UINT ObjectCBIndex = -1;

	Material* Mat;
	UINT TextSrvIndex = -1;

private:
	std::vector<Texture*> m_textures{};

};