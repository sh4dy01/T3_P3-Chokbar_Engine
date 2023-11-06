#include "BlankProject.h"
#include "Camera.h"

Camera::Camera()
{
	AddComponent<CameraComponent>();
}

Camera::~Camera()
{
}
