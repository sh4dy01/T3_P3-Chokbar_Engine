#include "BlankProject.h"

#include "Scripts/UI/ScoreBehavior.h"

#include "Score.h"

Score::Score()
{
	ScoreBehavior* scoreBehavior = NEW ScoreBehavior();
	AddComponent<ScoreBehavior>(scoreBehavior);

	for (int i = 0; i < NUM_DIGIT_PER_SCORE; i++)
	{
		scoreBehavior->CreateDigit(i);
	}

	scoreBehavior = nullptr;
}

Score::~Score()
{
}