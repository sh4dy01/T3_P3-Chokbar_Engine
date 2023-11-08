#pragma once
class DigitBehavior : public CustomComponent
{
public:
	DigitBehavior();
	~DigitBehavior();

	void Awake() override;
	void Start() override;
	void Update() override;

	void SetDigit(UINT digit);
	float CalculateUVOffsetFromDigit();

private:
	UIRenderer* m_uiRenderer;

	bool m_isDigitDirty;

	UINT m_digit;
};

