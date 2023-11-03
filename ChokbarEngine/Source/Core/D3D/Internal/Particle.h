#pragma once

struct InstanceData
{
	DirectX::XMFLOAT4X4 World = Identity4x4();
	float AgeRatio = 0.0f;
};

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
	void Init(float rLifeTime, DirectX::XMFLOAT3 rVel, DirectX::XMFLOAT3 rAngVel);

	float m_CurrentLifeTime;
	float m_LifeTime;

	DirectX::XMFLOAT3 m_Velocity;
	DirectX::XMFLOAT3 m_AngularVelocity;
private:
	bool m_IsActive;

	DirectX::XMFLOAT4 m_Color1;
	DirectX::XMFLOAT4 m_Color2;
};