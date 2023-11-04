#include "BlankProject.h"
#include "Asteroid.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	AddComponent<Rigidbody>();
	AddComponent<MeshRenderer>(new MeshRenderer(SPHERE, SIMPLE));
	AddComponent<SphereCollider>();
}

Asteroid::~Asteroid()
{
}

