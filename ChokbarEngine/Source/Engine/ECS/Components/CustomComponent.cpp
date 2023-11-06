#include "Chokbar.h"
#include "CustomComponent.h"

CustomComponent::CustomComponent()
{
	m_isCustomComponent = true;
}

CustomComponent::~CustomComponent()
{
}

void CustomComponent::OnAddedComponent()
{
	Engine::GetCoordinator()->RegisterCustomComponent(this);
}

void CustomComponent::OnRemovedComponent()
{
	Engine::GetCoordinator()->UnregisterCustomComponent(this);
}
