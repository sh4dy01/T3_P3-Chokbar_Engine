#include "BlankProject.h"



#include "DigitBehavior.h"

DigitBehavior::DigitBehavior() : m_isDigitDirty(false), m_digit(0)
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
	m_uiRenderer->OffsetUV(uvOffsetY);
}

void DigitBehavior::Start()
{
}

void DigitBehavior::Update()
{
	if (!m_isDigitDirty) return;

	float uvOffsetY = CalculateUVOffsetFromDigit();
	m_uiRenderer->OffsetUV(uvOffsetY);
	m_isDigitDirty = false;

}

void DigitBehavior::SetDigit(UINT digit)
{
	m_digit = digit;
	m_isDigitDirty = true;
}

float DigitBehavior::CalculateUVOffsetFromDigit()
{
	assert(m_digit < 10);
	return (0.1f * m_digit) + 0.1f;
}
