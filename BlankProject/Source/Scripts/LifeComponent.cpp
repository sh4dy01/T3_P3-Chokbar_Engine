#include "LifeComponent.h"


void LifeComponent::Start()
{
	m_CurrentLife = m_MaxLife;
}

void LifeComponent::RemoveLife(float damage)
{
	m_CurrentLife -= damage;

	if (m_CurrentLife <= 0)
	{
		m_CurrentLife = 0;
		OnDeath();
	}
}

void LifeComponent::InitMaxLife(float life)
{
	m_MaxLife = life;
	m_CurrentLife = life;
}
