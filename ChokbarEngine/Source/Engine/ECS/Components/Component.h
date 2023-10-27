#pragma once


namespace Chokbar
{
	class Transform;
	class GameObject;

	class Component
	{
	public:
		GameObject* gameObject;
		Transform* transform;
	};
}
