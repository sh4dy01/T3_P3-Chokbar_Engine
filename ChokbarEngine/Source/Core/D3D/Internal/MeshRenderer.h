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
	MeshRenderer(MeshType meshType, MaterialType matType);
	virtual ~MeshRenderer();

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

class ParticleRenderer : public MeshRenderer
{
public:
	ParticleRenderer();
	ParticleRenderer(MeshType meshType, MaterialType matType, UINT ParticleCount);
	~ParticleRenderer();

	void Awake(UINT instanceCount);
	void Update(float dt);

private:
	std::vector<ShaderParticle::InstanceData> m_particleInstanceData{};

	UINT m_particleCount = 0;
};