#pragma once
#include "Component.h"

class CustomComponent : public Component
{
public:

	CustomComponent();
	~CustomComponent() override;

	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void LateUpdate() {};

	//void OnEnable() override;
	//void OnDisable() override;


	void OnAddedComponent() override;
	void OnRemovedComponent() override;


};
