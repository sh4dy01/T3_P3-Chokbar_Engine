#pragma once
#include "Engine/ECS/Base/GameObject.h"
#include "Engine/ECS/Components/CameraComponent.h"


class Camera : public Chokbar::GameObject
{
public:

	Camera();
	~Camera();

	CameraComponent* GetCameraComponent() const { return m_CameraComponent; }

private:

	CameraComponent* m_CameraComponent;

};
