#pragma once

class Particle
{
public:
	Particle();
	~Particle();

	void Update(float deltaTime);

	bool IsAlive() const { return m_CurrentLifeTime < m_LifeTime; }
	bool IsActive() const { return m_IsActive; }
	void ToggleActivity() { m_IsActive = !m_IsActive; }

	void Reset();
	void Init(float rLifeTime, float rSpeed, DirectX::XMFLOAT3 rDir);

private:
	bool m_IsActive;

	float m_LifeTime;
	float m_CurrentLifeTime;

	float m_Speed;
	DirectX::XMFLOAT3 m_Direction;

	DirectX::XMFLOAT4 m_Color1;
	DirectX::XMFLOAT4 m_Color2;
	float m_opacity;
};