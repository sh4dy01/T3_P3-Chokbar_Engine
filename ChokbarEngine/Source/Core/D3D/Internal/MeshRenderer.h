#pragma once 
#include "Engine/ECS/Components/Component.h"

#include <DirectXCollision.h>

class Material;
class D3DMesh;
class Texture;

struct Volume {
	virtual bool isOnFrustum(const Frustum& camFrustum, const Transform& modelTransform) const = 0;
};

struct Sphere : public Volume
{
	bool isOnFrustum(const Frustum& camFrustum, Transform& transform) const final
	{
		//Get global scale is computed by doing the magnitude of
		//X, Y and Z model matrix's column.
		const XMFLOAT3 globalScale = transform.GetScale();

		//Get our global center with process it with the global model matrix of our transform
		const XMFLOAT3 globalCenter{ transform.GetPosition() * XMFLOAT4(center, 1.f) };

		//To wrap correctly our shape, we need the maximum scale scalar.
		const float maxScale = max(max(globalScale.x, globalScale.y), globalScale.z);

		//Max scale is assuming for the diameter. So, we need the half to apply it to our radius
		Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

		//Check Firstly the result that have the most chance
		//to faillure to avoid to call all functions.
		return (globalSphere.isOnOrForwardPlane(camFrustum.leftFace) &&
			globalSphere.isOnOrForwardPlane(camFrustum.rightFace) &&
			globalSphere.isOnOrForwardPlane(camFrustum.farFace) &&
			globalSphere.isOnOrForwardPlane(camFrustum.nearFace) &&
			globalSphere.isOnOrForwardPlane(camFrustum.topFace) &&
			globalSphere.isOnOrForwardPlane(camFrustum.bottomFace));
	};

	DirectX::XMFLOAT3 center{ 0.f, 0.f, 0.f };
	float radius = 0;
};

class MeshRenderer : public Component
{
public:

	MeshRenderer();
	~MeshRenderer() override;

	void Init(MeshType meshType, MaterialType matType);
	void OnAddedComponent() override;

	Texture* GetTexture(UINT index) const { return m_textures[index]; }
	std::vector<Texture*> GetTextures() const { return m_textures; }
	void RegisterTexture(Texture* tex);

protected:

	void OnDelete();

	void BindMaterial(Material* mat);

public:
	D3DMesh* Mesh;
	UINT ObjectCBIndex = -1;

	Sphere Bounds;

	Material* Mat;

private:
	std::vector<Texture*> m_textures{};
};
