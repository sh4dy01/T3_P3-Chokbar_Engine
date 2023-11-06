#include "Chokbar.h"

#include "IRenderer.h"

#include "D3D/Shaders/UploadBuffer.h"
#include "D3D/Shaders/Material.h"
#include "D3D/Shaders/ShaderBase.h"

#include "ParticleRenderer.h"

using namespace DirectX;

ParticleRenderer::ParticleRenderer() : IRenderer()
{
	srand(time(NULL));
	m_Color1 = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_Color2 = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

ParticleRenderer::~ParticleRenderer()
{
	for (auto& p : m_particles)
		DELPTR(p);
}


void ParticleRenderer::Init(MeshType meshType, MaterialType matType)
{
	IRenderer::Init(meshType, matType);

	m_particles = std::array<Particle*, MAX_PARTICLE_COUNT>{};
	m_particleInstanceData = std::array<InstanceData, MAX_PARTICLE_COUNT>{};

	Prepare();
}

void ParticleRenderer::Play()
{
	for (UINT i = 0; i < m_particleCount; i++)
		m_particles[i]->Awake();
}

void ParticleRenderer::Pause()
{
	for (UINT i = 0; i < m_particleCount; i++)
		m_particles[i]->Sleep();
}

void ParticleRenderer::Stop()
{
	for (UINT i = 0; i < MAX_PARTICLE_COUNT; i++)
		DELPTR(m_particles[i]);
}


void ParticleRenderer::SetParticleCount(UINT count)
{
	m_particleCount = count > MAX_PARTICLE_COUNT ? MAX_PARTICLE_COUNT : count;
	Prepare();
}

UINT ParticleRenderer::GetParticleCount() const
{
	return m_particleCount;
}


void ParticleRenderer::Render(ID3D12GraphicsCommandList* cmdList)
{
	if (!IsEnabled() || !Mat || !Mesh) return;

	auto shader = Mat->GetShader();
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

	float rLiftTime = rand() % 30 + 1.0f;

	float randomDirX = (((float)(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // Get a random number between -1 and 1
	float randomDirY = (((float)(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // ..
	float randomDirZ = (((float)(rand() % 100) * 0.01f) - 0.5f) * 2.0f; // ..
	DirectX::XMFLOAT3 rVel = { randomDirX, randomDirY, randomDirZ };

	float randomRotX = (((float)(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // Get a random number between -10 and 10
	float randomRotY = (((float)(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // ..
	float randomRotZ = (((float)(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // ..
	DirectX::XMFLOAT3 rAngVel = { randomRotX, randomRotY, randomRotZ };

	float randomScale = (((float)(rand() % 100) * 0.01f) * 0.25f) + 0.1f; // Get a random number between 0.1f and 0.35f
	p->m_Transform->SetScale(randomScale, randomScale, randomScale);

	p->Init(rLiftTime, rVel, rAngVel, transform->GetPosition());

	return p;
}


void ParticleRenderer::UpdateParticles(float dt)
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
			p->Sleep();
			p->Reset();
			continue;
		}

		p->m_CurrentLifeTime += dt;
		pid.AgeRatio = (p->m_LifeTime - p->m_CurrentLifeTime) / p->m_LifeTime;

		// Update position
		p->m_Transform->Translate(p->m_Velocity.x * dt, p->m_Velocity.y * dt, p->m_Velocity.z * dt);

		// Update rotation
		p->m_Transform->Rotate(p->m_AngularVelocity.x * dt, p->m_AngularVelocity.y * dt, p->m_AngularVelocity.z * dt);

		// Update InstanceData World matrix
		p->m_Transform->UpdateWorldMatrix();
		pid.World = *p->m_Transform->GetWorldMatrix();
		pid.Color1 = m_Color1;
		pid.Color2 = m_Color2;
	}

	UpdateShaderBuffer();
}

void ParticleRenderer::UpdateShaderBuffer()
{
	if (ShaderParticle* shader = dynamic_cast<ShaderParticle*>(Mat->GetShader()))
	{
		for (int i = 0; i < m_particleInstanceData.size(); i++)
		{
			shader->UpdateParticleInstanceDataBuffer(i, &m_particleInstanceData[i]);
		}
	}
}