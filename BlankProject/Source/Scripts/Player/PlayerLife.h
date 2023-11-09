#pragma once
#include "Scripts/LifeComponent.h"

class PlayerLife : public LifeComponent
{
protected:

	void Awake() override;
	void OnDeath() override;

private:


};

