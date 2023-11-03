#pragma once
#include "Engine/ECS/Components/CustomComponent.h"

class Player : public CustomComponent
{

	void Update() override;

private:

	m_Speed = 2.0f;

};