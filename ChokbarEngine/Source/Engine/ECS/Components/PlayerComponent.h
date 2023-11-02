#pragma once
#include "Component.h"

class PlayerComponent : public Component
{
public:

	~PlayerComponent() override;

	float Speed = 5.0f;

};


