#include "Chokbar.h"
#include "Component.h"

Component::~Component()
{
	transform = nullptr;
	gameObject = nullptr;
}
