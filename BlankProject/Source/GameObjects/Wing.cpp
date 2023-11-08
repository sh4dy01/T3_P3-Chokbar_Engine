#include "Wing.h"

Wing::Wing()
{
	AddComponent<MeshRenderer>()->Init(CUBE, SIMPLE);
	transform->SetScale(1, 1, 3);
}

Wing::~Wing()
{
}
