#pragma once
#include "Engine/ECS/Components/CustomComponent.h"

class ComponentTest : public CustomComponent
{
public:
	ComponentTest();
	~ComponentTest();

	void Awake() override;
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
};

