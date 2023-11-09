#include "BlankProject.h"

#include "Scripts/UI/DigitBehavior.h"

#include "Digit.h"

Digit::Digit()
{
	auto meshRenderer = AddComponent<UIRenderer>();
	meshRenderer->Init(MeshType::SQUARE, MaterialType::TEXTURE_OFFSET);
	meshRenderer->RegisterTexture(Resource::Load<Texture>("Resources/Textures/number.dds"));

	m_digitBehavior = AddComponent<DigitBehavior>();

	meshRenderer = nullptr;
}

Digit::~Digit()
{
	m_digitBehavior = nullptr;
}
