#pragma once 

class Material;

class MeshRenderer : public Chokbar::Component
{
public:

	MeshRenderer();
	~MeshRenderer();

	void BindMaterial(Material* mat);

	UINT ObjectCBIndex = -1;

public:
	D3DMesh* Mesh;
	Material* Mat;
};