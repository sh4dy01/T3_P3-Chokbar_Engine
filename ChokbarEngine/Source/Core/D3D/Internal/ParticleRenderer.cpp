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
	srand(time(NULL));
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

		float rLiftTime = rand() % 30 + 1.0f;

		float randomDirX = (((float)(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // Get a random number between -1 and 1
		float randomDirY = (((float)(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // ..
		float randomDirZ = (((float)(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // ..
		DirectX::XMFLOAT3 rVel = { randomDirX, randomDirY, randomDirZ };

		float randomRotX = (((float)(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // Get a random number between -10 and 10
		float randomRotY = (((float)(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // ..
		float randomRotZ = (((float)(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // ..
		DirectX::XMFLOAT3 rAngVel = { randomRotX, randomRotY, randomRotZ };

		float randomScale = ((float)(rand() % 100) * 0.01f) * 0.25f; // Get a random number between 0 and 0.25
		p->m_Transform->SetScale(randomScale, randomScale, randomScale);

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
		pid.AgeRatio =  (p->m_LifeTime - p->m_CurrentLifeTime) / p->m_LifeTime;

		// Update position
		float falseDt = 0.03f;
		p->m_Transform->Translate(p->m_Velocity.x * falseDt, p->m_Velocity.y * falseDt, p->m_Velocity.z * falseDt);

		// Update rotation
		p->m_Transform->Rotate(p->m_AngularVelocity.x * falseDt, p->m_AngularVelocity.y * falseDt, p->m_AngularVelocity.z * falseDt);

		// Update InstanceData World matrix
		p->m_Transform->UpdateWorldMatrix();
		pid.World = *p->m_Transform->GetWorldMatrix();
	}

	UpdateShaderBuffer();
}

void ParticleRenderer::UpdateShaderBuffer()
{
	if (ShaderParticle* shader = dynamic_cast<ShaderParticle*>(Mat->GetShader()))
	{
		for(int i = 0; i < m_particleInstanceData.size(); i++)
		{
			shader->UpdateParticleInstanceDataBuffer(i, &m_particleInstanceData[i]);
		}
	}
}