#include "Chokbar.h"
#include "CustomComponent.h"

#include "Engine/Engine.h"

CustomComponent::CustomComponent()
{
	m_isCustomComponent = true;
}

CustomComponent::~CustomComponent()
{
}

void CustomComponent::OnAddedComponent()
{
	Coordinator::GetInstance()->RegisterCustomComponent(this);
}

void CustomComponent::OnRemovedComponent()
{
	Coordinator::GetInstance()->UnregisterCustomComponent(this);
}
