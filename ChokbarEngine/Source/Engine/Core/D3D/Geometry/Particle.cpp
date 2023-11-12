#include "Chokbar.h"

#include "D3D/Base/D3DUtils.h"

#include "Particle.h"

#include <cmath>
using namespace DirectX;

#pragma region ParticleTransform
ParticleTransform::ParticleTransform() : m_dirty(false)
{
	// Initialize orientation vectors (right, up, forward)
	m_right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

	// Initialize position, scale, and rotation
	m_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&m_positionMatrix, DirectX::XMMatrixIdentity());
	m_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&m_scaleMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_rotationMatrix, DirectX::XMMatrixIdentity());
	m_rotationQuaternion = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	// Initialize rotation matrix as an identity matrix
	// Initialize world matrix as an identity matrix
	DirectX::XMStoreFloat4x4(&m_worldMatrix, DirectX::XMMatrixIdentity());
}

ParticleTransform::~ParticleTransform()
{
}

void ParticleTransform::Translate(float x, float y, float z)
{
	// Update the position vector
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	UpdatePositionMatrix();
}

void ParticleTransform::Translate(DirectX::XMFLOAT3 translation)
{
	Translate(translation.x, translation.y, translation.z);
}

void ParticleTransform::SetPosition(float x, float y, float z)
{
	// Set the position directly
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	UpdatePositionMatrix();
}

void ParticleTransform::SetPosition(DirectX::XMFLOAT3 newPosition)
{
	SetPosition(newPosition.x, newPosition.y, newPosition.z);
}

void ParticleTransform::RotateFromAxisAngle(DirectX::XMFLOAT3 axis, float angle)
{
	DirectX::XMVECTOR rotationDelta = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&axis), DirectX::XMConvertToRadians(angle));
	DirectX::XMVECTOR currentRotation = DirectX::XMLoadFloat4(&m_rotationQuaternion);
	currentRotation = DirectX::XMQuaternionMultiply(currentRotation, rotationDelta);

	// Store the result back in rotationQuaternion as XMFLOAT4
	DirectX::XMStoreFloat4(&m_rotationQuaternion, currentRotation);
	// Store the result back in rotationQuaternion as XMFLOAT4
	UpdateRotationMatrix();
	// Recalculate the world matrix
	m_right.x = m_rotationMatrix._11;
	m_right.y = m_rotationMatrix._12;
	m_right.z = m_rotationMatrix._13;
	m_up.x = m_rotationMatrix._21;
	m_up.y = m_rotationMatrix._22;
	m_up.z = m_rotationMatrix._23;
	m_forward.x = m_rotationMatrix._31;
	m_forward.y = m_rotationMatrix._32;
	m_forward.z = m_rotationMatrix._33;
}

void ParticleTransform::RotateYaw(float angle)
{
	RotateFromAxisAngle(m_up, angle);
}

void ParticleTransform::RotatePitch(float angle)
{
	RotateFromAxisAngle(m_right, angle);
}

void ParticleTransform::RotateRoll(float angle)
{
	RotateFromAxisAngle(m_forward, angle);
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
	m_scale.x *= x;
	m_scale.y *= y;
	m_scale.z *= z;
	UpdateScaleMatrix();
}

void ParticleTransform::Scale(DirectX::XMFLOAT3 scaleFactors)
{
	Scale(scaleFactors.x, scaleFactors.y, scaleFactors.z);
}

void ParticleTransform::SetScale(float x, float y, float z)
{
	// Set the scale factors directly
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;

	UpdateScaleMatrix();
}

void ParticleTransform::SetScale(DirectX::XMFLOAT3 scaleFactors)
{
	SetScale(scaleFactors.x, scaleFactors.y, scaleFactors.z);
}

void ParticleTransform::UpdatePositionMatrix()
{
	m_dirty = true;

	// Create the position matrix
	DirectX::XMStoreFloat4x4(&m_positionMatrix, DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
}

void ParticleTransform::UpdateRotationMatrix()
{
	m_dirty = true;
	// Create a quaternion for this rotation
	DirectX::XMVECTOR quaternion = DirectX::XMLoadFloat4(&m_rotationQuaternion);
	// Create a quaternion for this rotation
	// Convert the quaternion to a rotation matrix
	DirectX::XMStoreFloat4x4(&m_rotationMatrix, DirectX::XMMatrixRotationQuaternion(quaternion));
}

void ParticleTransform::UpdateScaleMatrix()
{
	m_dirty = true;
	// Convert the quaternion to a rotation matrix
	// Scale the orientation vectors
	DirectX::XMVECTOR scaledRight = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_right), m_scale.x);
	DirectX::XMVECTOR scaledUp = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_up), m_scale.y);
	DirectX::XMVECTOR scaledForward = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_forward), m_scale.z);

	// Create the scale matrix
	m_scaleMatrix = DirectX::XMFLOAT4X4(
		m_scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, m_scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, m_scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void ParticleTransform::UpdateWorldMatrix()
{
	// Create the position matrix
	// Combine rotation and scale and position
	DirectX::XMMATRIX newWorldMatrix = DirectX::XMLoadFloat4x4(&m_rotationMatrix) * DirectX::XMLoadFloat4x4(&m_scaleMatrix) * DirectX::XMLoadFloat4x4(&m_positionMatrix);
	// Combine rotation and scale with position
	// Convert the final world matrix to XMFLOAT4X4
	DirectX::XMStoreFloat4x4(&m_worldMatrix, DirectX::XMMatrixTranspose(newWorldMatrix));

	m_dirty = false;
}
#pragma endregion

#pragma region Particle
Particle::Particle()
	: LifeTime(0.0f), CurrentLifeTime(0.0f), Velocity(XMFLOAT3(0.0f, 0.0f, 0.0f)), AngularVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f)), m_isActive(false)
{
	Transform = new ParticleTransform();
}

Particle::~Particle()
{
	DELPTR(Transform);
}

void Particle::Update(float deltaTime)
{
	CurrentLifeTime -= deltaTime;
}

void Particle::Reset()
{
	CurrentLifeTime = 0.0f;

	AngularVelocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	Velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	Transform->SetPosition(0.0f, 0.0f, 0.0f);
	Transform->SetScale(1.0f, 1.0f, 1.0f);
	Transform->Rotate(0.0f, 0.0f, 0.0f);
}

void Particle::Init(float rLifeTime, DirectX::XMFLOAT3 rVel, DirectX::XMFLOAT3 rAngVel, DirectX::XMFLOAT3 parentPos)
{
	LifeTime = rLifeTime;
	CurrentLifeTime = 0.0f;

	AngularVelocity = rAngVel;
	Velocity = rVel;

	Transform->SetPosition(parentPos);
}
#pragma endregion