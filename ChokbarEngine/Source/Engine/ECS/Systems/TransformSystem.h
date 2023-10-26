#pragma once

namespace Chokbar {

	class TransformSystem : public System
	{
	public:
		TransformSystem() = default;
		~TransformSystem() = default;

	public:
		void Update() override;
	};

}