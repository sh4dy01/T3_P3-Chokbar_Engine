#include "BlankProject.h"

#include "Scripts/UI/DigitBehavior.h"

#include "Digit.h"

Digit::Digit()
{
	UIRenderer* meshRenderer = new UIRenderer();
	AddComponent<UIRenderer>(meshRenderer);
	meshRenderer->Init(MeshType::SQUARE, MaterialType::TEXTURE_OFFSET);
	meshRenderer->RegisterTexture(Resource::Load<Texture>("Resources/Textures/number.dds"));

	m_digitBehavior = NEW DigitBehavior();
	AddComponent<DigitBehavior>(m_digitBehavior);

	meshRenderer = nullptr;
}

Digit::~Digit()
{
	m_digitBehavior = nullptr;
}
