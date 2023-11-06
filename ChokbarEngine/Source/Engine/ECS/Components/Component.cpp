#include "Chokbar.h"
#include "Engine/ECS/Base/GameObject.h"
#include "Component.h"

Component::Component()
	:  gameObject(nullptr), transform(nullptr), m_isCustomComponent(false), m_isEnabled(true)
{
}

Component::~Component()
{
	transform = nullptr;
	gameObject = nullptr;
}
