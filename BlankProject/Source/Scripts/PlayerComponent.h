#pragma once

class PlayerComponent : public CustomComponent
{
public:
	
	PlayerComponent() = default;

	void Awake() override;
	void Start() override;
	void Update() override;


private:

	float m_Speed = 2.0f;

	CameraComponent* m_pCamera;
	Rigidbody* m_pRigidbody;
};