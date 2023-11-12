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
	m_color2 = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_color1 = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	m_particles.resize(0);
	m_particleInstanceData.resize(0);
}

ParticleRenderer::~ParticleRenderer()
{
	for (auto& p : m_particles)
		DELPTR(p)

	m_particles.clear();
	m_particleInstanceData.clear();
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

	m_particles.clear();
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
		if (m_particles[i] != nullptr) continue;

		CreateParticle();
	}
}

void ParticleRenderer::CreateParticle()
{
	auto* p = new Particle();

	const float rLiftTime = rand() % 3 + 1.0f;

	const float randomDirX = ((static_cast<float>(rand() % 100) * 0.25f) - 0.5f) * 2.0f; // Get a random number between -1 and 1
	const float randomDirY = ((static_cast<float>(rand() % 100) * 0.25f) - 0.5f) * 2.0f; // ..
	const float randomDirZ = ((static_cast<float>(rand() % 100) * 0.25f) - 0.5f) * 2.0f; // ..
	const XMFLOAT3 rVel = { randomDirX, randomDirY, randomDirZ };

	const float randomRotX = ((static_cast<float>(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // Get a random number between -10 and 10
	const float randomRotY = ((static_cast<float>(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // ..
	const float randomRotZ = ((static_cast<float>(rand() % 100) * 0.1f) - 5.0f) * 2.0f; // ..
	const XMFLOAT3 rAngVel = { randomRotX, randomRotY, randomRotZ };

	const float randomScale = ((static_cast<float>(rand() % 100) * 0.01f) * 5) + 0.1f; // Get a random number between 0.1f and 0.35f
	p->Transform->SetScale(randomScale, randomScale, randomScale);

	p->Init(rLiftTime, rVel, rAngVel, transform->GetPosition());
	p->Awake();
	
	m_particles.push_back(p);
}


void ParticleRenderer::UpdateParticles(const float dt)
{
	for (UINT i = 0; i < m_particleCount; i++)
	{
		Particle* p = m_particles[i];

		// Since some particles might be deleted, we need to check if the pointer is still valid
		if (p == nullptr) continue;

		// If the particle is dead, delete it
		if (!p->IsAlive() || !p->IsActive())
		{
			DELPTR(p)
			m_particles.erase(m_particles.begin() + i);
			m_particleInstanceData.erase(m_particleInstanceData.begin() + i);
			m_particleCount--;
			if (i > 0)
				i--;
			continue;
		}

		// Get the reference to the particle's InstanceData to be copied to the shader buffer
		auto& [World, Color1, Color2, AgeRatio] = m_particleInstanceData[i];

		// Update particle's lifetime and InstanceData AgeRatio
		p->CurrentLifeTime += dt;
		AgeRatio = (p->LifeTime - p->CurrentLifeTime) / p->LifeTime;

		// Update position with particle's velocity
		p->Transform->Translate(p->Velocity.x * dt, p->Velocity.y * dt, p->Velocity.z * dt);

		// Update rotation with particle's angular velocity
		p->Transform->Rotate(p->AngularVelocity.x * dt, p->AngularVelocity.y * dt, p->AngularVelocity.z * dt);

		// Update InstanceData World matrix
		p->Transform->UpdateWorldMatrix();
		World = *p->Transform->GetWorldMatrix();
		
		Color1 = m_color1;
		Color2 = m_color2;
	}

	UpdateShaderBuffer();
}

void ParticleRenderer::UpdateShaderBuffer() const
{
	// Make sure the shader is a ParticleShader, otherwise we can't update the InstanceData buffer
	if (const auto shader = dynamic_cast<ShaderParticle*>(Mat->GetShader()))
	{
		// Update the InstanceData buffer for each particle
		for (int i = 0; i < static_cast<int>(m_particleInstanceData.size()); i++)
		{
			shader->UpdateParticleInstanceDataBuffer(i, &m_particleInstanceData[i]);
		}
	}
}
