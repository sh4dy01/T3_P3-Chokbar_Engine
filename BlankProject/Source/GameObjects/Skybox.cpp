#include "BlankProject.h"
#include "SkyBox.h"

SkyBox::SkyBox()
{
	m_Name = "SkyBox";

	SkyRenderer* sr = new SkyRenderer();
	sr->Init(MeshType::CUBE, MaterialType::SKYBOX);
	std::string path = "Resources/Textures/spaceCM.dds";
	sr->RegisterTexture(Resource::Load<Texture>(path));

	AddComponent<SkyRenderer>(sr);

	transform->SetScale(10.0f, 10.0f, 10.0f);
	transform->SetPosition(0.0f, 0.0f, 0.0f);
}

SkyBox::~SkyBox()
{
}

