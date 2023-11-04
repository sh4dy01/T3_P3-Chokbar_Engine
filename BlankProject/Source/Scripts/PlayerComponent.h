#pragma once

class PlayerComponent : public CustomComponent
{
public:
	
	PlayerComponent() = default;

	void Awake() override;
	void Start() override;
	void Update() override;


private:

	float m_Speed = 4.0f;
	int zIncrement = 5;

	CameraComponent* m_pCamera;

};