#include "Wing.h"

Wing::Wing()
{
	AddComponent<MeshRenderer>()->Init(CUBE, SIMPLE);
	transform->SetScale(0.5f, 0.5f, 3);
}

Wing::~Wing()
{
}
