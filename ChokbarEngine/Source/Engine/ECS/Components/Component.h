#pragma once


namespace Chokbar
{
	class Transform;
	class GameObject;

	class Component
	{
	public:
		virtual void OnAddedComponent() {};

		GameObject* gameObject;
		Transform* transform;
	};
}
