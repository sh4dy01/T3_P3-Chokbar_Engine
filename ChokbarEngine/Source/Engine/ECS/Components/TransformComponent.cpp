#include "Chokbar.h"
#include "TransformComponent.h"

namespace Chokbar
{
	Transform::Transform()
	{
		// Initialize position, scale, and rotation
		position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

		// Initialize orientation vectors (right, up, forward)
		right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

		// Initialize rotation matrix as an identity matrix
		DirectX::XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixIdentity());

		// Initialize rotation quaternion as an identity quaternion
		rotationQuaternion = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

		// Initialize world matrix as an identity matrix
		//worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	}

	void Transform::Translate(float x, float y, float z)
	{
		// Update the position vector
		position.x += x;
		position.y += y;
		position.z += z;

		// Recalculate the world matrix
		GetWorldMatrix();
	}
	void Transform::Translate(DirectX::XMFLOAT3 translation)
	{
		// Update the position vector using the provided translation vector
		position.x += translation.x;
		position.y += translation.y;
		position.z += translation.z;

		// Recalculate the world matrix
		GetWorldMatrix();
	}
	void Transform::SetPosition(float x, float y, float z)
	{
		// Set the position directly
		position.x = x;
		position.y = y;
		position.z = z;

		// Recalculate the world matrix
		GetWorldMatrix();
	}
	void Transform::SetPosition(DirectX::XMFLOAT3 newPosition)
	{
		// Set the position using the provided vector
		position = newPosition;

		// Recalculate the world matrix
		GetWorldMatrix();
	}

	void Transform::RotateFromAxisAngle(DirectX::XMFLOAT3 axis, float angle)
	{
		DirectX::XMVECTOR rotationDelta = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&axis), DirectX::XMConvertToRadians(angle));
		DirectX::XMVECTOR currentRotation = DirectX::XMLoadFloat4(&rotationQuaternion);
		currentRotation = DirectX::XMQuaternionMultiply(currentRotation, rotationDelta);

		// Store the result back in rotationQuaternion as XMFLOAT4
		DirectX::XMStoreFloat4(&rotationQuaternion, currentRotation);

		// Recalculate the world matrix
		GetWorldMatrix();

		right.x = rotationMatrix._11;
		right.y = rotationMatrix._12;
		right.z = rotationMatrix._13;
		up.x = rotationMatrix._21;
		up.y = rotationMatrix._22;
		up.z = rotationMatrix._23;
		forward.x = rotationMatrix._31;
		forward.y = rotationMatrix._32;
		forward.z = rotationMatrix._33;
	}

	void Transform::RotateYaw(float angle)
	{
		RotateFromAxisAngle(up, angle);
	}
	void Transform::RotatePitch(float angle)
	{
		RotateFromAxisAngle(right, angle);
	}
	void Transform::RotateRoll(float angle)
	{
		RotateFromAxisAngle(forward, angle);
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
		scale.x *= x;
		scale.y *= y;
		scale.z *= z;

		// Recalculate the world matrix
		GetWorldMatrix();
	}
	void Transform::Scale(DirectX::XMFLOAT3 scaleFactors)
	{
		// Update the scale factors using the provided vector
		scale.x *= scaleFactors.x;
		scale.y *= scaleFactors.y;
		scale.z *= scaleFactors.z;

		// Recalculate the world matrix
		GetWorldMatrix();
	}
	void Transform::SetScale(float x, float y, float z)
	{
		// Set the scale factors directly
		scale.x = x;
		scale.y = y;
		scale.z = z;

		// Recalculate the world matrix
		GetWorldMatrix();
	}
	void Transform::SetScale(DirectX::XMFLOAT3 scaleFactors)
	{
		// Set the scale factors using the provided vector
		scale = scaleFactors;

		// Recalculate the world matrix
		GetWorldMatrix();
	}

	DirectX::XMFLOAT3 Transform::GetPosition()
	{
		return position;
	}
	DirectX::XMFLOAT3 Transform::GetScale()
	{
		return scale;
	}

	DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
	{
		// Create a quaternion for this rotation
		DirectX::XMVECTOR quaternion = DirectX::XMLoadFloat4(&rotationQuaternion);

		// Convert the quaternion to a rotation matrix
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);

		// Scale the orientation vectors
		DirectX::XMVECTOR scaledRight = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&right), scale.x);
		DirectX::XMVECTOR scaledUp = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&up), scale.y);
		DirectX::XMVECTOR scaledForward = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&forward), scale.z);

		// Create the scale matrix
		DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixIdentity();
		scalingMatrix.r[0] = scaledRight;
		scalingMatrix.r[1] = scaledUp;
		scalingMatrix.r[2] = scaledForward;

		// Create the position matrix
		DirectX::XMMATRIX positionMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		// Combine rotation and scale with position
		DirectX::XMMATRIX worldMatrix = rotationMatrix * scalingMatrix * positionMatrix;

		// Convert the final world matrix to XMFLOAT4X4
		DirectX::XMFLOAT4X4 worldMatrixFloat4x4;
		DirectX::XMStoreFloat4x4(&worldMatrixFloat4x4, DirectX::XMMatrixTranspose(worldMatrix));

		return worldMatrixFloat4x4;
	}
}