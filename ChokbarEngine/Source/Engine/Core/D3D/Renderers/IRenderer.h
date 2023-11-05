#pragma once

class Material;
class D3DMesh;
class Texture;

struct ID3D12GraphicsCommandList;

class IRenderer
{
	friend class D3DRenderer;

public:
	IRenderer();
	virtual ~IRenderer();

public:
	virtual void Init(MeshType meshType, MaterialType matType);

	Texture* GetTexture(UINT index) const { return m_textures[index]; }
	std::vector<Texture*> GetTextures() const { return m_textures; }
	void RegisterTexture(Texture* tex);

public:
	D3DMesh* Mesh;
	UINT ObjectCBIndex = -1;

	Material* Mat;

protected:
	void OnDelete();
	void BindMaterial(Material* mat);

	virtual void Render(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual void Update(float dt) = 0;
protected:
	std::vector<Texture*> m_textures{};

private:

};