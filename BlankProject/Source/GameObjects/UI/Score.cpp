#include "BlankProject.h"

#include "Scripts/UI/ScoreBehavior.h"

#include "Score.h"

Score::Score()
{
	AddComponent<ScoreBehavior>();
}

Score::~Score()
{
}