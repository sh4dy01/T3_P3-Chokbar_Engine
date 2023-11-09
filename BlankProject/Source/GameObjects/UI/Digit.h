#pragma once

class DigitBehavior;

class Digit : public GameObject
{
public:
	Digit();
	~Digit();

	DigitBehavior* GetDigitBehavior() const { return m_digitBehavior; }

private:
	DigitBehavior* m_digitBehavior;
};

