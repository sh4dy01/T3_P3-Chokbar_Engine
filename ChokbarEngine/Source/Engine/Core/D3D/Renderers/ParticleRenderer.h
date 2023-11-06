#pragma once

#include "D3D/Geometry/Particle.h"

#define MAX_PARTICLE_COUNT 1000

// Double inheritance is not a good idea, I know.
class ParticleRenderer : public Component, public IRenderer
{
public:
	ParticleRenderer();
	~ParticleRenderer();

	void Init(MeshType meshType, MaterialType matType) override;
	void Render(ID3D12GraphicsCommandList* cmdList) override;
	void Update(float dt) override;

	void SetParticleCount(UINT count);
	UINT GetParticleCount() const;

	std::array<InstanceData, MAX_PARTICLE_COUNT>& GetParticleInstanceData();

private:
	std::array<Particle*, MAX_PARTICLE_COUNT> m_particles{};
	std::array<InstanceData, MAX_PARTICLE_COUNT> m_particleInstanceData{};

	UINT m_particleCount = 0;

	void Regenerate();
	void UpdateParticles(float dt);
	void UpdateShaderBuffer();
};