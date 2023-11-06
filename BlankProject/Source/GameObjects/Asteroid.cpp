#include "BlankProject.h"
#include "Asteroid.h"

#include "Engine/Resource.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	AddComponent<Rigidbody>();
	MeshRenderer* meshRenderer = new MeshRenderer();
	meshRenderer->Init(MeshType::SPHERE, MaterialType::SIMPLE);
	AddComponent<MeshRenderer>(new MeshRenderer());
	AddComponent<SphereCollider>();
}

Asteroid::~Asteroid()
{
}

