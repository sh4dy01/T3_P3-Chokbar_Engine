#include "PlayerLife.h"
#include "Engine/Engine.h"

void PlayerLife::OnDeath()
{
	Engine::RestartGame();
}
