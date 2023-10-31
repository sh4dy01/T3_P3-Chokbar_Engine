#include "Chokbar.h"
#include "TransformComponent.h"

#include <numbers>

Transform::Transform()
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

void Transform::Translate(float x, float y, float z)
{
	// Update the position vector
	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;
	UpdatePositionMatrix();
}

void Transform::Translate(DirectX::XMFLOAT3 translation)
{
	Translate(translation.x, translation.y, translation.z);
}

void Transform::SetPosition(float x, float y, float z)
{
	// Set the position directly
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
	UpdatePositionMatrix();
}

void Transform::SetPosition(DirectX::XMFLOAT3 newPosition)
{
	SetPosition(newPosition.x, newPosition.y, newPosition.z);
}

void Transform::RotateFromAxisAngle(DirectX::XMFLOAT3 axis, float angle)
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

void Transform::RotateYaw(float angle)
{
	RotateFromAxisAngle(m_Up, angle);
}

void Transform::RotatePitch(float angle)
{
	RotateFromAxisAngle(m_Right, angle);
}

void Transform::RotateRoll(float angle)
{
	RotateFromAxisAngle(m_Forward, angle);
}

void Transform::Rotate(float pitch = 0.f, float yaw = 0.f, float roll = 0.f)
{
	RotatePitch(pitch);
	RotateYaw(yaw);
	RotateRoll(roll);
}

void Transform::Rotate(DirectX::XMFLOAT3 rotation)
{
	Rotate(rotation.x, rotation.y, rotation.z);
}

void Transform::Scale(float x, float y, float z)
{
	// Update the scale factors
	m_Scale.x *= x;
	m_Scale.y *= y;
	m_Scale.z *= z;
	UpdateScaleMatrix();
}

void Transform::Scale(DirectX::XMFLOAT3 scaleFactors)
{
	Scale(scaleFactors.x, scaleFactors.y, scaleFactors.z);
}

void Transform::SetScale(float x, float y, float z)
{
	// Set the scale factors directly
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
		
	UpdateScaleMatrix();
}

void Transform::SetScale(DirectX::XMFLOAT3 scaleFactors)
{
	SetScale(scaleFactors.x, scaleFactors.y, scaleFactors.z);
}

void Transform::UpdatePositionMatrix()
{
	m_Dirty = true;

	// Create the position matrix
	DirectX::XMStoreFloat4x4(&m_PositionMatrix, DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z));
}
void Transform::UpdateRotationMatrix()
{
	m_Dirty = true;
	// Create a quaternion for this rotation
	DirectX::XMVECTOR quaternion = DirectX::XMLoadFloat4(&m_RotationQuaternion);
	// Create a quaternion for this rotation
	// Convert the quaternion to a rotation matrix
	DirectX::XMStoreFloat4x4(&m_RotationMatrix, DirectX::XMMatrixRotationQuaternion(quaternion));
}

void Transform::UpdateScaleMatrix()
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

void Transform::UpdateWorldMatrix()
{
	// Create the position matrix
	// Combine rotation and scale and position
	DirectX::XMMATRIX newWorldMatrix = DirectX::XMLoadFloat4x4(&m_RotationMatrix) * DirectX::XMLoadFloat4x4(&m_ScaleMatrix) * DirectX::XMLoadFloat4x4(&m_PositionMatrix);
	// Combine rotation and scale with position
	// Convert the final world matrix to XMFLOAT4X4
	DirectX::XMStoreFloat4x4(&m_WorldMatrix, DirectX::XMMatrixTranspose(newWorldMatrix));

	m_Dirty = false;
}
