#pragma once

namespace Chokbar {

	class TransformSystem : public System
	{
	public:
		TransformSystem() = default;
		~TransformSystem() = default;

	public:
		void Awake() override;
		void Start() override;
		void Update() override;
	};

}