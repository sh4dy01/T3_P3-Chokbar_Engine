#pragma once 

class Material;
class D3DMesh;

class MeshRenderer : public Component
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