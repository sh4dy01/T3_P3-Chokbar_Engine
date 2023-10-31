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

	Texture* GetTexture(UINT index) const { return Textures[index]; }

	UINT ObjectCBIndex = -1;

public:
	D3DMesh* Mesh;
	Material* Mat;
	std::vector<Texture*> Textures{};
};