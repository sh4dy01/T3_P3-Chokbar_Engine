#pragma once
#include "Engine/ECS/Components/Component.h"


class TriggerCollisionEvent : public Component {
public:

	TriggerCollisionEvent();
	~TriggerCollisionEvent() override;

	void OnAddedComponent() override;

	virtual void OnTriggerEnter(Collider* other) = 0;
	//virtual void OnTriggerStay(Collider* other) {};
	//virtual void OnTriggerExit(Collider* other) {};

};