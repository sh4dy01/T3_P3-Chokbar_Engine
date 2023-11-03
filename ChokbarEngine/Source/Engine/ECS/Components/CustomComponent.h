#pragma once
#include "Engine/ECS/Components/Component.h"

class CustomComponent : public Component
{
public:

	CustomComponent();
	~CustomComponent() override;

	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
	virtual void LateUpdate() = 0;

	//void OnEnable() override;
	//void OnDisable() override;


	void OnAddedComponent() override;
	void OnRemovedComponent() override;

};