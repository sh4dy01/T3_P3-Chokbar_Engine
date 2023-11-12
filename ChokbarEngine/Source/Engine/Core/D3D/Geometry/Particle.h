#pragma once

struct InstanceData
{
	DirectX::XMFLOAT4X4 World = Identity4X4();
	DirectX::XMFLOAT4 Color1 = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 Color2 = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	float AgeRatio = 0.0f;
};

// ParticleTransform class that holds the position, rotation and scale of a particle
// Note that we do not used the Transform class, we do not need to component part of the transform
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

	DirectX::XMFLOAT3 GetPosition() const {return m_position;}
	DirectX::XMFLOAT3 GetScale() const { return m_scale; }
	DirectX::XMFLOAT4 GetQuaternion() const { return m_rotationQuaternion; }

	DirectX::XMFLOAT4X4* GetWorldMatrix() { return &m_worldMatrix ;}

	bool IsDirty() const { return m_dirty; }

	void UpdateWorldMatrix();

private:

	void RotateFromAxisAngle(DirectX::XMFLOAT3 axis, float angle);

	void UpdatePositionMatrix();
	void UpdateRotationMatrix();
	void UpdateScaleMatrix();

	bool m_dirty;

	DirectX::XMFLOAT3 m_right;
	DirectX::XMFLOAT3 m_up;
	DirectX::XMFLOAT3 m_forward;

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT4X4 m_positionMatrix;

	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT4X4 m_scaleMatrix;

	DirectX::XMFLOAT4X4 m_rotationMatrix;
	DirectX::XMFLOAT4 m_rotationQuaternion;

	DirectX::XMFLOAT4X4 m_worldMatrix;
};

// Particle class that holds all the data for a single particle
// This class is only used by the ParticleRenderer class 
class Particle
{
public:
	Particle();
	~Particle();

	void Update(float deltaTime);

	bool IsAlive() const { return CurrentLifeTime < LifeTime; }
	bool IsActive() const { return m_isActive; }

	void Sleep() { m_isActive = false; }
	void Awake() { m_isActive = true; }

	void Reset();
	void Init(float rLifeTime, DirectX::XMFLOAT3 rVel, DirectX::XMFLOAT3 rAngVel, DirectX::XMFLOAT3 parentPos);



	float CurrentLifeTime;
	float LifeTime;

	DirectX::XMFLOAT3 Velocity;
	DirectX::XMFLOAT3 AngularVelocity;

	ParticleTransform* Transform;

private:
	bool m_isActive;
};
