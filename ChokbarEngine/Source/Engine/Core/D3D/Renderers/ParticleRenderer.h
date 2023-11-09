#pragma once

#include "D3D/Geometry/Particle.h"

#define MAX_PARTICLE_COUNT 10

// Double inheritance is not a good idea, I know.
class ParticleRenderer : public Component, public IRenderer
{
	// Only D3DRenderer can Update and Render this component.
	friend class D3DRenderer;
public:
	ParticleRenderer();
	~ParticleRenderer() override;

	void Play() const;
	void Pause() const;
	void Stop();

	void Init(MeshType meshType, MaterialType matType) override;

	void SetParticleCount(UINT count);
	void AddParticles(UINT count);
	UINT GetParticleCount() const;

	DirectX::XMFLOAT4 GetColor1() { return m_Color1; }
	DirectX::XMFLOAT4 GetColor2() { return m_Color2; }

private:
	std::vector<Particle*> m_particles{};
	std::vector<InstanceData> m_particleInstanceData{};

	UINT m_particleCount = 0;

	DirectX::XMFLOAT4 m_Color1;
	DirectX::XMFLOAT4 m_Color2;

	void Render(ID3D12GraphicsCommandList* cmdList) override;
	void Update(float dt) override;

	void Prepare();
	void CreateMissingParticles();
	Particle* CreateParticle();

	void UpdateParticles(float dt);
	void UpdateShaderBuffer() const;
};