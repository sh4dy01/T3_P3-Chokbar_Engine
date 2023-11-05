#pragma once 
#include "Engine/ECS/Components/Component.h"

class Material;
class D3DMesh;
class Texture;

class MeshRenderer : public Component
{
public:

	MeshRenderer();
	~MeshRenderer() override;

	virtual void Init(MeshType meshType, MaterialType matType);

	Texture* GetTexture(UINT index) const { return m_textures[index]; }
	std::vector<Texture*> GetTextures() const { return m_textures; }
	void RegisterTexture(Texture* tex);

protected:
	void OnDelete();

	void BindMaterial(Material* mat);

public:
	D3DMesh* Mesh;
	UINT ObjectCBIndex = -1;

	Material* Mat;

private:
	std::vector<Texture*> m_textures{};
};
