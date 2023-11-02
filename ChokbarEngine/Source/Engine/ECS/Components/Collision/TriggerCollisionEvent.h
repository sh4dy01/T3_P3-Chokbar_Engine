#pragma once
#include "Engine/ECS/Components/Component.h"

class TriggerCollisionEvent : public Component {
public:

	TriggerCollisionEvent();
	~TriggerCollisionEvent() override;

	void OnAddedComponent() override;

	virtual void OnTriggerEnter(CollisionShape* other) = 0;
	virtual void OnTriggerStay(CollisionShape* other) = 0;
	virtual void OnTriggerExit(CollisionShape* other) = 0;

};