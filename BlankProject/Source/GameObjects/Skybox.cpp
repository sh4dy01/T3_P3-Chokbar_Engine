#include "BlankProject.h"
#include "SkyBox.h"

SkyBox::SkyBox()
{
	m_Name = "SkyBox";

	/*
	SkyRenderer* sr = new SkyRenderer();
	sr->Init(MeshType::SPHERE, MaterialType::SKYBOX);
	std::string path = "Resources/Textures/cubemap.dds";
	sr->RegisterTexture(Resource::Load<Texture>(path));

	AddComponent<SkyRenderer>(sr);

	transform->SetScale(500.f, 500.0f, 500.0f);
	transform->SetPosition(0.0f, 0.0f, 0.0f);
	*/

	auto mr = AddComponent<MeshRenderer>();
	mr->Init(MeshType::SPHERE, MaterialType::SKYBOX);
	mr->RegisterTexture(Resource::Load<Cubemap>("Resources/Textures/cubemap.dds"));
	

	transform->SetScale(500.f, 500.0f, 500.0f);
	transform->SetPosition(0.0f, 0.0f, 0.0f);

	mr = nullptr;
}

SkyBox::~SkyBox()
{
}

