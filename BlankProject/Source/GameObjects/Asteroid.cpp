#include "BlankProject.h"
#include "Asteroid.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	AddComponent<Rigidbody>();

	auto mesh = AddComponent<MeshRenderer>();
	mesh->Init(SPHERE, TEXTURE);
	mesh->RegisterTexture(Resource::Load<Texture>("Resources/Textures/mars.dds"));

	AddComponent<SphereCollider>();

	mesh = nullptr;
}

Asteroid::~Asteroid()
{
}

