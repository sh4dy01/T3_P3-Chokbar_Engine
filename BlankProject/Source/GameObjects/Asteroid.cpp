#include "BlankProject.h"
#include "Asteroid.h"

#include "Engine/Resource.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	AddComponent<Rigidbody>();
	std::string path = "Resources/Textures/mars.dds";

	auto mr = AddComponent<MeshRenderer>(new MeshRenderer(SPHERE, TEXTURE));
	mr->RegisterTexture(Resource::Load<Texture>(path));

	AddComponent<SphereCollider>();

	mr = nullptr;
}

Asteroid::~Asteroid()
{
}

