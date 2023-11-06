#include "Chokbar.h"

#include "Engine/Particles/Particle.h"
#include "Core/D3D/Internal/MeshRenderer.h"
#include "Core/D3D/Internal/D3DMesh.h"

#include "Engine/Resource.h"

#include "ParticlesRenderer.h"

ParticlesRenderer::ParticlesRenderer()
{
	m_MeshRenderer = new MeshRenderer(MeshType::CUBE, MaterialType::SIMPLE);
}

ParticlesRenderer::~ParticlesRenderer()
{
	for (auto& v : m_ParticleVertices)
		DELPTR(v);

	for (auto& i : m_ParticleIndices)
		DELPTR(i);
}

void ParticlesRenderer::Awake()
{
	for (auto& v : m_ParticleVertices)
		v = new Vertex();

	for (auto& i : m_ParticleIndices)
		i = new UINT();
}

void ParticlesRenderer::Start()
{
}

void ParticlesRenderer::Update(float deltaTime)
{

}

void ParticlesRenderer::CreateGlobalMesh(std::array<Particle*, MAX_PARTICLES>& ar)
{
}

void ParticlesRenderer::RecreateGlobalMeshGPU()
{
}