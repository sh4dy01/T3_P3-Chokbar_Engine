#pragma once
#include "Engine/ECS/Components/Component.h"


class TriggerCollisionEvent : public Component {
public:

	TriggerCollisionEvent();
	~TriggerCollisionEvent() override;

	void OnAddedComponent() override;

	virtual void OnTriggerEnter(Collider* other) = 0;
	virtual void OnTriggerStay(Collider* other) = 0;
	virtual void OnTriggerExit(Collider* other) = 0;

};