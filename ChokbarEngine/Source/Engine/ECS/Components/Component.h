#pragma once


struct Transform;

namespace Chokbar
{
	class GameObject;

	struct Component
	{
		GameObject* gameObject;
		Transform* transform;
	};
}
