#pragma once

#include <DirectXMath.h>

#include "Component.h"

// Transform is a component and is meant to be attached to 3D objects to give them a position, rotation and scale and to be able to move and rotate them around.


class Transform : public Chokbar::Component
{
public:
	Transform();

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

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();

	DirectX::XMFLOAT4X4 GetWorldMatrix();

private:
	void RotateFromAxisAngle(DirectX::XMFLOAT3 axis, float angle);

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT3 right;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 forward;

	DirectX::XMFLOAT4X4 rotationMatrix;
	DirectX::XMFLOAT4 rotationQuaternion;

	DirectX::XMFLOAT4X4 worldMatrix;
};
