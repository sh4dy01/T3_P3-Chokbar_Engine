#include "BlankProject.h"
#include "Asteroid.h"
#include "Engine/Resource.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	AddComponent<Rigidbody>();
	std::string path = "Resources/Textures/mars.dds";
	MeshRenderer* meshRenderer = new MeshRenderer();
	meshRenderer->Init(MeshType::SPHERE, MaterialType::TEXTURE);
	meshRenderer->RegisterTexture(Resource::Load<Texture>(path));
	AddComponent<MeshRenderer>(meshRenderer);
	AddComponent<SphereCollider>();
}

Asteroid::~Asteroid()
{
}