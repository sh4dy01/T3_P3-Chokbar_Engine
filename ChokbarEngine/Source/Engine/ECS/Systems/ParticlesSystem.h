#pragma once

class ParticleSystem : public System
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;

private:
	std::array<Particle*, MAX_PARTICLES> m_Particles; 
	ParticlesRenderer* m_ParticlesRenderer;
};