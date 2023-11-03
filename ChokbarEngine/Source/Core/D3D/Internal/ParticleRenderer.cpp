#include "Chokbar.h"

#include "MeshRenderer.h"
#include "UploadBuffer.h"
#include "Particle.h"

#include "Material.h"
#include "ShaderBase.h"

#include "ParticleRenderer.h"

using namespace DirectX;

ParticleRenderer::ParticleRenderer() : MeshRenderer()
{

}

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::SetParticleCount(UINT count)
{
	m_particleCount = count;
	Regenerate();
}

UINT ParticleRenderer::GetParticleCount() const
{
	return m_particleCount;
}

std::array<InstanceData, MAX_PARTICLE_COUNT>& ParticleRenderer::GetParticleInstanceData()
{
	return m_particleInstanceData;
}

void ParticleRenderer::Regenerate()
{
	m_particles = std::array<Particle*, MAX_PARTICLE_COUNT>{};
	m_particleInstanceData = std::array<InstanceData, MAX_PARTICLE_COUNT>{};

	for (auto& p : m_particles)
	{
		p = new Particle();

		float rLiftTime = rand() % 4 + 1.0f;

		float randomDirX = (((float)(rand() % 100) / 100.0f) - 0.5f) * 2.0f; // Get a random number between -1 and 1
		float randomDirY = (((float)(rand() % 100) / 100.0f) - 0.5f) * 2.0f; // ..
		float randomDirZ = (((float)(rand() % 100) / 100.0f) - 0.5f) * 2.0f; // ..
		DirectX::XMFLOAT3 rVel = { randomDirX, randomDirY, randomDirZ };

		float randomRotX = (((float)(rand() % 100) / 10.0f) - 5.0f) * 2.0f; // Get a random number between -10 and 10
		float randomRotY = (((float)(rand() % 100) / 10.0f) - 5.0f) * 2.0f; // ..
		float randomRotZ = (((float)(rand() % 100) / 10.0f) - 5.0f) * 2.0f; // ..
		DirectX::XMFLOAT3 rAngVel = { randomRotX, randomRotY, randomRotZ };

		p->Init(rLiftTime, rVel, rAngVel);
	}

	UpdateShaderBuffer();
}

void ParticleRenderer::Update(float dt)
{
	for (UINT i = 0; i < MAX_PARTICLE_COUNT; i++)
	{
		Particle* p = m_particles[i];
		// We skip all inactive particles. This particle renderer does not support dynamic particle generation. 
		// If you want to do that, you need to implement a particle pool.
		if (!p->IsActive()) continue;

		InstanceData& pid = m_particleInstanceData[i];

		if (!p->IsAlive())
		{
			p->Reset();
			p->ToggleActivity();
			continue;
		}

		p->m_CurrentLifeTime += dt;
		pid.AgeRatio = p->m_CurrentLifeTime / p->m_LifeTime;

		XMFLOAT3 dtVel3x3, dtRot3x3;

		XMStoreFloat3(&dtVel3x3, XMLoadFloat3(&p->m_Velocity) * dt);
		XMStoreFloat3(&dtRot3x3, XMLoadFloat3(&p->m_AngularVelocity) * dt);

		XMMATRIX transfo = XMMatrixRotationRollPitchYaw(dtRot3x3.x, dtRot3x3.y, dtRot3x3.z) * XMMatrixTranslation(dtVel3x3.x, dtVel3x3.y, dtVel3x3.z);
		XMStoreFloat4x4(&pid.World, XMMatrixTranspose(transfo));
	}

	UpdateShaderBuffer();
}

void ParticleRenderer::UpdateShaderBuffer()
{
	if (ShaderParticle* shader = dynamic_cast<ShaderParticle*>(Mat->GetShader()))
	{
		shader->UpdateParticleInstanceDataBuffer(0, m_particleInstanceData.data());
	}
}