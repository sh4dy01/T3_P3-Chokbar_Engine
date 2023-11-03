#pragma once 
#include "Engine/ECS/Components/Component.h"

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
	~MeshRenderer() override;

	void OnDelete();

	void BindMaterial(Material* mat);

	Texture* GetTexture(UINT index) const { return m_textures[index]; }
	std::vector<Texture*> GetTextures() const { return m_textures; }
	void RegisterTexture(Texture* tex);


public:
	D3DMesh* Mesh;
	UINT ObjectCBIndex = -1;

	Material* Mat;
	UINT TextSrvIndex = -1;

private:
	std::vector<Texture*> m_textures{};

};