#pragma once

class Material;
class D3DMesh;
class Texture;

struct ID3D12GraphicsCommandList;

/*
Base class for all Render components
It manages to communication between Shader classes, Mesh classes and D3DRenderer class
*/
class IRenderer
{
public:
	IRenderer();
	virtual ~IRenderer();

public:
	virtual void Init(const MeshType meshType, const MaterialType matType);

	[[nodiscard]] Texture* GetTexture(const UINT index) const { return m_textures[index]; }
	[[nodiscard]] std::vector<Texture*> GetTextures() const { return m_textures; }
	void RegisterTexture(Texture* tex);

public:
	D3DMesh* Mesh;
	UINT ObjectCBIndex = -1;

	Material* Mat;

protected:
	void OnDelete() const;
	void BindMaterial(Material* mat);

	virtual void Render(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual void Update(float dt) = 0;
protected:
	std::vector<Texture*> m_textures{};

private:

};