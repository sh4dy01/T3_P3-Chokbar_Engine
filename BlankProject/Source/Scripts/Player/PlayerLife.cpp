#include "PlayerLife.h"
#include "Engine/Engine.h"


void PlayerLife::Awake()
{
	InitMaxLife(1);
}

void PlayerLife::OnDeath()
{
	Engine::RestartGame();
}
