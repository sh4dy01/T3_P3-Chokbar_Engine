#pragma once 

class Material;
class D3DMesh;
class Texture;

class MeshRenderer : public Chokbar::Component
{
public:

	MeshRenderer();
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