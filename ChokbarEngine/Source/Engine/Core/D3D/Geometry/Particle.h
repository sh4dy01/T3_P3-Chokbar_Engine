#pragma once

struct InstanceData
{
	DirectX::XMFLOAT4X4 World = Identity4x4();
	DirectX::XMFLOAT4 Color1 = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 Color2 = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	float AgeRatio = 0.0f;
};


class ParticleTransform
{
public:
	ParticleTransform();
	~ParticleTransform();

public:
	void Translate(float x, float y, float z);
	void Translate(DirectX::XMFLOAT3 translation);

	void RotateYaw(float angle);
	void RotatePitch(float angle);
	void RotateRoll(float angle);

	void Rotate(float yaw, float pitch, float roll);
	void Rotate(DirectX::XMFLOAT3 rotation);

	void Scale(float x, float y, float z);
	void Scale(DirectX::XMFLOAT3 scale);

	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 position);

	void SetScale(float x, float y, float z);
	void SetScale(DirectX::XMFLOAT3 scale);

	DirectX::XMFLOAT3 GetPosition() {return m_Position;}
	DirectX::XMFLOAT3 GetScale() const { return m_Scale; }
	DirectX::XMFLOAT4 GetQuaternion() const { return m_RotationQuaternion; }

	DirectX::XMFLOAT4X4* GetWorldMatrix() { return &m_WorldMatrix ;}

	bool IsDirty() const { return m_Dirty; }

	void UpdateWorldMatrix();

private:

	void RotateFromAxisAngle(DirectX::XMFLOAT3 axis, float angle);

	void UpdatePositionMatrix();
	void UpdateRotationMatrix();
	void UpdateScaleMatrix();

	bool m_Dirty;

	DirectX::XMFLOAT3 m_Right;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Forward;

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT4X4 m_PositionMatrix;

	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT4X4 m_ScaleMatrix;

	DirectX::XMFLOAT4X4 m_RotationMatrix;
	DirectX::XMFLOAT4 m_RotationQuaternion;

	DirectX::XMFLOAT4X4 m_WorldMatrix;
};

class Particle
{
public:
	Particle();
	~Particle();

	void Update(float deltaTime);

	bool IsAlive() const { return m_currentLifeTime < m_lifeTime; }
	bool IsActive() const { return m_isActive; }

	void Sleep() { m_isActive = false; }
	void Awake() { m_isActive = true; }

	void Reset();
	void Init(float rLifeTime, DirectX::XMFLOAT3 rVel, DirectX::XMFLOAT3 rAngVel, DirectX::XMFLOAT3 parentPos);



	float m_currentLifeTime;
	float m_lifeTime;

	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_angularVelocity;

	ParticleTransform* m_transform;

private:
	bool m_isActive;
};
