#include "Chokbar.h"

#include "IRenderer.h"

#include "D3D/Shaders/UploadBuffer.h"
#include "D3D/Shaders/Material.h"
#include "D3D/Shaders/ShaderBase.h"

#include "ParticleRenderer.h"

using namespace DirectX;

ParticleRenderer::ParticleRenderer() : IRenderer()
{
	srand(time(nullptr));
	m_Color1 = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_Color2 = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	m_particles.resize(0);
	m_particleInstanceData.resize(0);
}

ParticleRenderer::~ParticleRenderer()
{
	for (auto& p : m_particles)
		DELPTR(p)
}


void ParticleRenderer::Init(const MeshType meshType, const MaterialType matType)
{
	IRenderer::Init(meshType, matType);
}

void ParticleRenderer::Play() const
{
	for (UINT i = 0; i < m_particleCount; i++)
		m_particles[i]->Awake();
}

void ParticleRenderer::Pause() const
{
	for (UINT i = 0; i < m_particleCount; i++)
		m_particles[i]->Sleep();
}

void ParticleRenderer::Stop()
{
	for (UINT i = 0; i < MAX_PARTICLE_COUNT; i++)
		DELPTR(m_particles[i])
}


void ParticleRenderer::SetParticleCount(UINT count)
{
	m_particleCount = count > MAX_PARTICLE_COUNT ? MAX_PARTICLE_COUNT : count;
	Prepare();
}

void ParticleRenderer::AddParticles(UINT count)
{
	m_particleCount = (m_particleCount + count > MAX_PARTICLE_COUNT) ? MAX_PARTICLE_COUNT : m_particleCount + count;
	Prepare();
}

UINT ParticleRenderer::GetParticleCount() const
{
	return m_particleCount;
}


void ParticleRenderer::Render(ID3D12GraphicsCommandList* cmdList)
{
	if (!IsEnabled() || !Mat || !Mesh) return;

	const auto shader = Mat->GetShader();
	shader->BeginDraw(cmdList);

	shader->Draw(cmdList, this);

	shader->EndDraw(cmdList);
}

void ParticleRenderer::Update(float dt)
{
	if (!IsEnabled() || !Mat || !Mesh) return;

	UpdateParticles(dt);
}


void ParticleRenderer::Prepare()
{
	CreateMissingParticles();

	UpdateShaderBuffer();
}

void ParticleRenderer::CreateMissingParticles()
{
	m_particles.resize(m_particleCount, nullptr);
	m_particleInstanceData.resize(m_particleCount);
	
	for (UINT i = 0; i < m_particleCount; i++)
	{
		// Skip all created particles
		if (m_particles[i] != nullptr) continue;

		m_particles[i] = CreateParticle();
	}
}

Particle* ParticleRenderer::CreateParticle()
{
	Particle* p = new Particle();

	const float rLiftTime = rand() % 30 + 1.0f;

	const float randomDirX = ((static_cast<float>(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // Get a random number between -1 and 1
	const float randomDirY = ((static_cast<float>(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // ..
	const float randomDirZ = ((static_cast<float>(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // ..
	const XMFLOAT3 rVel = { randomDirX, randomDirY, randomDirZ };

	const float randomRotX = ((static_cast<float>(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // Get a random number between -10 and 10
	const float randomRotY = ((static_cast<float>(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // ..
	const float randomRotZ = ((static_cast<float>(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // ..
	const XMFLOAT3 rAngVel = { randomRotX, randomRotY, randomRotZ };

	const float randomScale = ((static_cast<float>(rand() % 100) * 0.01f) * 0.25f) + 0.1f; // Get a random number between 0.1f and 0.35f
	p->m_transform->SetScale(randomScale, randomScale, randomScale);

	p->Init(rLiftTime, rVel, rAngVel, transform->GetPosition());

	return p;
}


void ParticleRenderer::UpdateParticles(const float dt)
{
	for (UINT i = 0; i < m_particleCount; i++)
	{
		Particle* p = m_particles[i];
		// We skip all inactive particles. This particle renderer does not support dynamic particle generation. 
		// If you want to do that, you need to implement a particle pool.
		if (p == nullptr || !p->IsActive()) continue;

		InstanceData& pid = m_particleInstanceData[i];


		if (!p->IsAlive())
		{
			//DELPTR(p);
			p->Sleep();
			p->Reset();
			continue;
		}

		p->m_currentLifeTime += dt;
		pid.AgeRatio = (p->m_lifeTime - p->m_currentLifeTime) / p->m_lifeTime;

		// Update position
		p->m_transform->Translate(p->m_velocity.x * dt, p->m_velocity.y * dt, p->m_velocity.z * dt);

		// Update rotation
		p->m_transform->Rotate(p->m_angularVelocity.x * dt, p->m_angularVelocity.y * dt, p->m_angularVelocity.z * dt);

		// Update InstanceData World matrix
		p->m_transform->UpdateWorldMatrix();
		pid.World = *p->m_transform->GetWorldMatrix();
		pid.Color1 = m_Color1;
		pid.Color2 = m_Color2;
	}

	UpdateShaderBuffer();
}

void ParticleRenderer::UpdateShaderBuffer() const
{
	if (const auto shader = dynamic_cast<ShaderParticle*>(Mat->GetShader()))
	{
		for (int i = 0; i < static_cast<int>(m_particleInstanceData.size()); i++)
		{
			shader->UpdateParticleInstanceDataBuffer(i, &m_particleInstanceData[i]);
		}
	}
}
