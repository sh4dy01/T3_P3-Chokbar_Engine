#include "Chokbar.h"

#include "Core/D3D/MeshType.h"
#include "Core/D3D/MaterialType.h"

#include "Engine/Resource.h"
#include "Engine/ECS/Components/Component.h"
#include "D3DMesh.h"
#include "Core/D3D/D3DMath.h"

#include "Material.h"
#include "ShaderBase.h"
#include "Texture.h"

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
