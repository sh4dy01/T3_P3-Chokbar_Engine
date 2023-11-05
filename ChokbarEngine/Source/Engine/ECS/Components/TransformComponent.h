#pragma once

#include "Engine/ECS/Components/Component.h"

// Transform is a component and is meant to be attached to 3D objects to give them a position, rotation and scale and to be able to move and rotate them around.


class Transform : public Component
{
public:
	enum Space
	{
		Local,
		World
	};
public:
	Transform();
	~Transform() override;

	void Translate(float x, float y, float z, Space space = Space::Local);
	void Translate(DirectX::XMFLOAT3 translation, Space space = Space::Local);
	void Translate(DirectX::XMVECTOR translation, Space space = Space::Local);

	void RotateYaw(float angle, Space space = Space::Local);
	void RotatePitch(float angle, Space space = Space::Local);
	void RotateRoll(float angle, Space space = Space::Local);

	void Rotate(float yaw, float pitch, float roll, Space space = Space::Local);
	void Rotate(DirectX::XMFLOAT3 rotation, Space space = Space::Local);

	void Scale(float x, float y, float z);
	void Scale(DirectX::XMFLOAT3 scale);

	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 position);

	void SetScale(float x, float y, float z);
	void SetScale(DirectX::XMFLOAT3 scale);

	DirectX::XMFLOAT3 GetEulerAngles();

	DirectX::XMFLOAT3 GetRight() { return m_Right; }
	DirectX::XMFLOAT3 GetUp() { return m_Up; }
	DirectX::XMFLOAT3 GetForward() { return m_Forward; }

	DirectX::XMFLOAT3 GetPosition() { return m_Position; }
	DirectX::XMFLOAT3 GetScale() const { return m_Scale; }
	DirectX::XMFLOAT4 GetQuaternion() const { return m_RotationQuaternion; }

	DirectX::XMFLOAT4X4* GetWorldMatrix() { return &m_WorldMatrix; }

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