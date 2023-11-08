#pragma once
#include "../Asteroids/AsteroidLife.h"

class AsteroidCollisionEvent : public TriggerCollisionEvent
{
public:
	AsteroidCollisionEvent() {};
	~AsteroidCollisionEvent() override;
	// temp
	void OnAddedComponent() override;
	void OnRemovedComponent() override {};	

	void OnTriggerEnter(Collider* other) override final;
	//void OnTriggerStay(Collider* other) override;
	//void OnTriggerExit(Collider* other) override;

private:

	AsteroidLife* m_AsteroidLife;

};


