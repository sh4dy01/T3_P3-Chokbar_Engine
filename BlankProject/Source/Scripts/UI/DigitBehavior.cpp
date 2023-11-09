#include "BlankProject.h"



#include "DigitBehavior.h"

DigitBehavior::DigitBehavior() : m_isDigitDirty(false), m_digit(0), m_prevOffsetY(0.0f)
{
	m_uiRenderer = nullptr;
}

DigitBehavior::~DigitBehavior()
{
	m_uiRenderer = nullptr;
}

void DigitBehavior::Awake()
{
	float uvOffsetY = CalculateUVOffsetFromDigit();

	m_uiRenderer = gameObject->GetComponent<UIRenderer>();
	m_uiRenderer->SetOffsetY(uvOffsetY);
}

void DigitBehavior::Start()
{

}

void DigitBehavior::Update()
{
	if (!m_isDigitDirty) return;

	// Store the previous offset
	m_prevOffsetY = m_uiRenderer->GetUVOffsetY();

	// Calculate the new offset and set it
	float uvOffsetY = CalculateUVOffsetFromDigit() * TimeManager::GetDeltaTime() * 2.0f;
	m_uiRenderer->AddOffsetY(uvOffsetY);

	// Since we are scrolling the texture with delta time, we do not know when to stop.
	// We check if the new offset is a multiple of 0.1f, which is the size of each digit.
	float newOffsetY = m_uiRenderer->GetUVOffsetY();
	if (fmod(newOffsetY, 0.1f) < 0.0001f)
	{
		m_isDigitDirty = false;
	}
}

void DigitBehavior::SetDigit(UINT digit)
{
	m_isDigitDirty = (m_digit == digit) ? false : true;
	m_digit = digit;
}

float DigitBehavior::CalculateUVOffsetFromDigit()
{
	assert(m_digit < 10);
	return (0.1f * m_digit);
}
