#pragma once

class LifeComponent : public CustomComponent
{
public:

	void InitMaxLife(float life);
	void RemoveLife(float damage);

	float GetCurrentLife() const { return m_CurrentLife; };
	float GetMaxLife() const { return m_MaxLife; };

protected:

	virtual void OnDeath() = 0;

protected:

	float m_CurrentLife;
	float m_MaxLife = 100;

	
};

