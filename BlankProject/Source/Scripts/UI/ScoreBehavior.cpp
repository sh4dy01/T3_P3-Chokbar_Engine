#include "BlankProject.h"

#include "GameObjects/UI/Digit.h"
#include "Scripts/UI/DigitBehavior.h"

#include "ScoreBehavior.h"

ScoreBehavior::ScoreBehavior() : m_score(0), m_isScoreDirty(false)
{

}

ScoreBehavior::~ScoreBehavior()
{
	OnDelete();
}


void ScoreBehavior::Awake()
{

}

void ScoreBehavior::Start()
{

}

void ScoreBehavior::Update()
{
	//if (!m_isScoreDirty) return;

	UpdateScoreDisplay();
	m_isScoreDirty = false;
}

void ScoreBehavior::UpdateScore(UINT newScore)
{
	m_score = newScore;
	m_isScoreDirty = true;
}

void ScoreBehavior::CreateDigit(UINT index)
{
	Digit* digit = GameObject::Instantiate<Digit>();
	float startX = 0.0f - (NUM_DIGIT_PER_SCORE * 0.5f * 0.1f) + 0.05f;
	digit->transform->SetPosition(startX + 0.10f * index, 0.91f, 0);

	digitsObjects[index] = digit;
}

void ScoreBehavior::OnDelete()
{
	for (GameObject* digit : digitsObjects)
		digit->Destroy();
}

void ScoreBehavior::UpdateScoreDisplay()
{
	std::string scoreString = std::to_string(m_score);
	UINT scoreStringLength = scoreString.length();

	for (UINT i = scoreStringLength; i < NUM_DIGIT_PER_SCORE; ++i)
	{
		scoreString = "0" + scoreString;
	}

	for (UINT i = 0; i < NUM_DIGIT_PER_SCORE; ++i)
	{
		UINT digit = scoreString[i] - '0';
		digitsObjects[i]->GetComponent<DigitBehavior>()->SetDigit(digit);
	}
}

