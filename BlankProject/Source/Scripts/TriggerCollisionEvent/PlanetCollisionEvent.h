#pragma once
class PlanetCollisionEvent : public TriggerCollisionEvent
{
	public:

	PlanetCollisionEvent();
	~PlanetCollisionEvent() override;

	void OnAddedComponent() override;
	void OnTriggerEnter(Collider* other) override;

	void GetParticleRenderer();

private:

	ParticleRenderer * m_ParticleRenderer = nullptr;

};

