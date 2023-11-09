#pragma once
#include "Scripts/Player/PlayerLife.h"

class PlayerCollisionEvent : public TriggerCollisionEvent
{
public:
	~PlayerCollisionEvent() override;

	void OnAddedComponent() override;
	void OnTriggerEnter(Collider* other) override;

private:

	PlayerLife* m_PlayerLife;

};

