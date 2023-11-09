#include "PlayerLife.h"
#include "Engine/Engine.h"


void PlayerLife::Awake()
{
	InitMaxLife(3);
}

void PlayerLife::OnDeath()
{
	Engine::RestartGame();
}
