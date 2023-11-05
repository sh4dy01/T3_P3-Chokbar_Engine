#include "Chokbar.h"

#include "Engine/Resource.h"
#include "Engine/ECS/Components/Component.h"

#include "D3D/Geometry/D3DMesh.h"
#include "D3D/Geometry/GeometryHandler.h"

#include "D3D/Shaders/Material.h"
#include "D3D/Shaders/ShaderBase.h"
#include "D3D/Shaders/Texture.h"

#include "MeshRenderer.h"

using namespace DirectX;

MeshRenderer::MeshRenderer()
{
	Mesh = nullptr;
	Mat = nullptr;
}

MeshRenderer::~MeshRenderer()
{
	OnDelete();

	Mesh = nullptr;
	Mat = nullptr;
	m_textures.clear();
}

void MeshRenderer::Init(MeshType meshType, MaterialType matType)
{
	Mesh = GeometryHandler::GetMesh(meshType);

	Material* mat = Resource::LoadMaterial(matType);
	BindMaterial(mat);
}

void MeshRenderer::OnDelete()
{
	Mat->GetShader()->UnBind(ObjectCBIndex);
}

void MeshRenderer::RegisterTexture(Texture* tex)
{
	m_textures.push_back(tex);
}

void MeshRenderer::BindMaterial(Material* mat)
{
	Mat = mat;
	ObjectCBIndex = Mat->GetShader()->Bind()->GetCreatedIndex();
}
