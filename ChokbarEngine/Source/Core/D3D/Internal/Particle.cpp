#include "Chokbar.h"

#include "Particle.h"

#include <cmath>
using namespace DirectX;

#pragma region ParticleTransform
ParticleTransform::ParticleTransform() : m_Dirty(false)
{
	// Initialize orientation vectors (right, up, forward)
	m_Right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_Up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

	// Initialize position, scale, and rotation
	m_Position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&m_PositionMatrix, DirectX::XMMatrixIdentity());
	m_Scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&m_ScaleMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_RotationMatrix, DirectX::XMMatrixIdentity());
	m_RotationQuaternion = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	// Initialize rotation matrix as an identity matrix
	// Initialize world matrix as an identity matrix
	DirectX::XMStoreFloat4x4(&m_WorldMatrix, DirectX::XMMatrixIdentity());
}

void ParticleTransform::Translate(float x, float y, float z)
{
	// Update the position vector
	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;
	UpdatePositionMatrix();
}

void ParticleTransform::Translate(DirectX::XMFLOAT3 translation)
{
	Translate(translation.x, translation.y, translation.z);
}

void ParticleTransform::SetPosition(float x, float y, float z)
{
	// Set the position directly
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
	UpdatePositionMatrix();
}

void ParticleTransform::SetPosition(DirectX::XMFLOAT3 newPosition)
{
	SetPosition(newPosition.x, newPosition.y, newPosition.z);
}

void ParticleTransform::RotateFromAxisAngle(DirectX::XMFLOAT3 axis, float angle)
{
	DirectX::XMVECTOR rotationDelta = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&axis), DirectX::XMConvertToRadians(angle));
	DirectX::XMVECTOR currentRotation = DirectX::XMLoadFloat4(&m_RotationQuaternion);
	currentRotation = DirectX::XMQuaternionMultiply(currentRotation, rotationDelta);

	// Store the result back in rotationQuaternion as XMFLOAT4
	DirectX::XMStoreFloat4(&m_RotationQuaternion, currentRotation);
	// Store the result back in rotationQuaternion as XMFLOAT4
	UpdateRotationMatrix();
	// Recalculate the world matrix
	m_Right.x = m_RotationMatrix._11;
	m_Right.y = m_RotationMatrix._12;
	m_Right.z = m_RotationMatrix._13;
	m_Up.x = m_RotationMatrix._21;
	m_Up.y = m_RotationMatrix._22;
	m_Up.z = m_RotationMatrix._23;
	m_Forward.x = m_RotationMatrix._31;
	m_Forward.y = m_RotationMatrix._32;
	m_Forward.z = m_RotationMatrix._33;
}

void ParticleTransform::RotateYaw(float angle)
{
	RotateFromAxisAngle(m_Up, angle);
}

void ParticleTransform::RotatePitch(float angle)
{
	RotateFromAxisAngle(m_Right, angle);
}

void ParticleTransform::RotateRoll(float angle)
{
	RotateFromAxisAngle(m_Forward, angle);
}

void ParticleTransform::Rotate(float pitch = 0.f, float yaw = 0.f, float roll = 0.f)
{
	RotatePitch(pitch);
	RotateYaw(yaw);
	RotateRoll(roll);
}

void ParticleTransform::Rotate(DirectX::XMFLOAT3 rotation)
{
	Rotate(rotation.x, rotation.y, rotation.z);
}

void ParticleTransform::Scale(float x, float y, float z)
{
	// Update the scale factors
	m_Scale.x *= x;
	m_Scale.y *= y;
	m_Scale.z *= z;
	UpdateScaleMatrix();
}

void ParticleTransform::Scale(DirectX::XMFLOAT3 scaleFactors)
{
	Scale(scaleFactors.x, scaleFactors.y, scaleFactors.z);
}

void ParticleTransform::SetScale(float x, float y, float z)
{
	// Set the scale factors directly
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;

	UpdateScaleMatrix();
}

void ParticleTransform::SetScale(DirectX::XMFLOAT3 scaleFactors)
{
	SetScale(scaleFactors.x, scaleFactors.y, scaleFactors.z);
}

void ParticleTransform::UpdatePositionMatrix()
{
	m_Dirty = true;

	// Create the position matrix
	DirectX::XMStoreFloat4x4(&m_PositionMatrix, DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z));
}

void ParticleTransform::UpdateRotationMatrix()
{
	m_Dirty = true;
	// Create a quaternion for this rotation
	DirectX::XMVECTOR quaternion = DirectX::XMLoadFloat4(&m_RotationQuaternion);
	// Create a quaternion for this rotation
	// Convert the quaternion to a rotation matrix
	DirectX::XMStoreFloat4x4(&m_RotationMatrix, DirectX::XMMatrixRotationQuaternion(quaternion));
}

void ParticleTransform::UpdateScaleMatrix()
{
	m_Dirty = true;
	// Convert the quaternion to a rotation matrix
	// Scale the orientation vectors
	DirectX::XMVECTOR scaledRight = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_Right), m_Scale.x);
	DirectX::XMVECTOR scaledUp = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_Up), m_Scale.y);
	DirectX::XMVECTOR scaledForward = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_Forward), m_Scale.z);

	// Create the scale matrix
	m_ScaleMatrix = DirectX::XMFLOAT4X4(
		m_Scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, m_Scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, m_Scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void ParticleTransform::UpdateWorldMatrix()
{
	// Create the position matrix
	// Combine rotation and scale and position
	DirectX::XMMATRIX newWorldMatrix = DirectX::XMLoadFloat4x4(&m_RotationMatrix) * DirectX::XMLoadFloat4x4(&m_ScaleMatrix) * DirectX::XMLoadFloat4x4(&m_PositionMatrix);
	// Combine rotation and scale with position
	// Convert the final world matrix to XMFLOAT4X4
	DirectX::XMStoreFloat4x4(&m_WorldMatrix, DirectX::XMMatrixTranspose(newWorldMatrix));

	m_Dirty = false;
}
#pragma endregion

#pragma region Particle
Particle::Particle()
	: m_LifeTime(0.0f), m_CurrentLifeTime(0.0f), m_Velocity(XMFLOAT3(0.0f, 0.0f, 0.0f)), m_AngularVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f)), m_IsActive(false)
{
	m_Color1 = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Color2 = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	m_Transform = new ParticleTransform();
}

Particle::~Particle()
{
}

void Particle::Update(float deltaTime)
{
	m_CurrentLifeTime -= deltaTime;

	/*
	_position += _direction * (_speed * Time.deltaTime);
	Vector3 rotation = _rotation.eulerAngles;
	rotation.x += 10 * Time.deltaTime;
	rotation.y += 10 * Time.deltaTime;
	rotation.z += 10 * Time.deltaTime;
	_rotation = Quaternion.Euler(rotation);


	_particleMaterial.color = Color.Lerp(_color1, _color2, particle.CurrentLifeTime / particle.LifeTime);
	particle.Render(_particleMesh, _particleMaterial);
	_particleMaterial.color = _color1;
	*/
}

void Particle::Reset()
{
	m_CurrentLifeTime = 0.0f;
	m_AngularVelocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Particle::Init(float rLifeTime, DirectX::XMFLOAT3 rVel, DirectX::XMFLOAT3 rAngVel)
{
	m_LifeTime = rLifeTime;
	m_CurrentLifeTime = 0.0f;

	m_AngularVelocity = rAngVel;
	m_Velocity = rVel;

	m_IsActive = true;
}
#pragma endregion