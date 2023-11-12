#include "Chokbar.h"

#include "Engine/Resource.h"

#include "D3D/Geometry/D3DMesh.h"
#include "D3D/Geometry/GeometryHandler.h"

#include "D3D/Shaders/Material.h"
#include "D3D/Shaders/ShaderBase.h"
#include "D3D/Shaders/Texture.h"

#include "IRenderer.h"

IRenderer::IRenderer()
{
	Mesh = nullptr;
	Mat = nullptr;
}

IRenderer::~IRenderer()
{
	OnDelete();

	Mesh = nullptr;
	Mat = nullptr;
	m_textures.clear();
}

void IRenderer::Init(const MeshType meshType, const MaterialType matType)
{
	Mesh = GeometryHandler::GetMesh(meshType);

	Material* mat = Resource::LoadMaterial(matType);
	BindMaterial(mat);
}

void IRenderer::OnDelete() const
{
	if (Mat)
		Mat->GetShader()->UnBind(ObjectCBIndex);
}

void IRenderer::RegisterTexture(Texture* tex)
{
	assert(tex != nullptr);

	m_textures.push_back(tex);
}

void IRenderer::BindMaterial(Material* mat)
{
	Mat = mat;
	ObjectCBIndex = Mat->GetShader()->Bind()->GetCreatedIndex();
}