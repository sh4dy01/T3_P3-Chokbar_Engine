#include "Chokbar.h"

#include "Engine/Resource.h"
#include "Engine/ECS/Components/Component.h"
#include "D3DMesh.h"
#include "../D3DMath.h"

#include "Material.h"
#include "ShaderBase.h"
#include "Texture.h"

#include "MeshRenderer.h"

using namespace DirectX;


#pragma region MeshRenderer
MeshRenderer::MeshRenderer()
{
	Mesh = nullptr;
	Mat = nullptr;
}

MeshRenderer::MeshRenderer(MeshType meshType, MaterialType matType)
{
	Mesh = GeometryHandler::GetMesh(meshType);

	Material* mat = Resource::LoadMaterial(matType);
	BindMaterial(mat);
}

MeshRenderer::~MeshRenderer()
{
	OnDelete();

	Mesh = nullptr;
	Mat = nullptr;
	m_textures.clear();
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
#pragma endregion

#pragma region ParticleRenderer
ParticleRenderer::ParticleRenderer() : MeshRenderer()
{
}

ParticleRenderer::ParticleRenderer(MeshType meshType, MaterialType matType, UINT particleCount) : MeshRenderer(meshType, matType), m_particleCount(particleCount)
{
	Awake(particleCount);
}

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::Awake(UINT instanceCount)
{
	m_particleInstanceData.resize(instanceCount);
	for (auto& pid : m_particleInstanceData)
	{
		pid.Age = 0.0f;

		pid.LifeTime = rand() % 4 + 1.0f;

		float randomDirX = (((float)(rand() % 100) / 100.0f) - 0.5f) * 2.0f; // Get a random number between -1 and 1
		float randomDirY = (((float)(rand() % 100) / 100.0f) - 0.5f) * 2.0f; // ..
		float randomDirZ = (((float)(rand() % 100) / 100.0f) - 0.5f) * 2.0f; // ..
		pid.Velocity = { randomDirX, randomDirY, randomDirZ };

		float randomRotX = (((float)(rand() % 100) / 10.0f) - 5.0f) * 2.0f; // Get a random number between -10 and 10
		float randomRotY = (((float)(rand() % 100) / 10.0f) - 5.0f) * 2.0f; // ..
		float randomRotZ = (((float)(rand() % 100) / 10.0f) - 5.0f) * 2.0f; // ..
		pid.Rotation = { randomRotX, randomRotY, randomRotZ };

		
	}
}

void ParticleRenderer::Update(float dt)
{
	for (auto& pid : m_particleInstanceData)
	{
		pid.Age += dt;

		XMFLOAT3 dtVel3x3, dtRot3x3;
		XMVECTOR dtVel = XMLoadFloat3(&pid.Velocity) * dt;
		XMVECTOR dtRot = XMLoadFloat3(&pid.Rotation) * dt;

		XMStoreFloat3(&dtVel3x3, dtVel);
		XMStoreFloat3(&dtRot3x3, dtRot);

		XMMATRIX transfo = XMMatrixRotationRollPitchYaw(dtRot3x3.x, dtRot3x3.y, dtRot3x3.z) * XMMatrixTranslation(dtVel3x3.x, dtVel3x3.y, dtVel3x3.z);
		XMStoreFloat4x4(&pid.World, transfo);
		XMStoreFloat4x4(&pid.World, XMMatrixTranspose(XMLoadFloat4x4(&pid.World)));
	}
}
#pragma endregion