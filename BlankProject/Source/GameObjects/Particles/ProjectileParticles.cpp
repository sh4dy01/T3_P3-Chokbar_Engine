#include "ProjectileParticles.h"

ProjectileParticles::ProjectileParticles()
{
	m_Name = "Particles";

	const auto pr = AddComponent<ParticleRenderer>();
	pr->Init(MeshType::SPHERE, MaterialType::PARTICLE);
	//pr->SetParticleCount(20);
	//pr->transform->SetScale(0.1f);
}

ProjectileParticles::~ProjectileParticles()
{
}
