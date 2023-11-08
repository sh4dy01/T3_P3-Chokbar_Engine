#include "BlankProject.h"
#include "Asteroid.h"
#include "Engine/Resource.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	AddComponent<Rigidbody>();

	auto mesh = AddComponent<MeshRenderer>();
	mesh->Init(SPHERE, SIMPLE);

	AddComponent<SphereCollider>();

	mesh = nullptr;
}

Asteroid::~Asteroid()
{
}