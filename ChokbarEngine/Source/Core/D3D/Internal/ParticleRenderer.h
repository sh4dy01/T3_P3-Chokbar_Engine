#pragma once

#include "Particle.h"

#define MAX_PARTICLE_COUNT 1000

class ParticleRenderer : public MeshRenderer
{
public:
	ParticleRenderer();
	~ParticleRenderer();

	void Init(MeshType meshType, MaterialType matType) override;
	void Update(float dt);

	void SetParticleCount(UINT count);
	UINT GetParticleCount() const;

	std::array<InstanceData, MAX_PARTICLE_COUNT>& GetParticleInstanceData();

private:
	std::array<Particle*, MAX_PARTICLE_COUNT> m_particles{};
	std::array<InstanceData, MAX_PARTICLE_COUNT> m_particleInstanceData{};

	UINT m_particleCount = 0;

	void Regenerate();
	void UpdateShaderBuffer();
};