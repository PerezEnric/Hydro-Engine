#include "GameTimer.h"

GameTimer::GameTimer()
{
}

GameTimer::~GameTimer()
{
}

void GameTimer::Start()
{
	is_running = true;
	game_timer.Start();
}

void GameTimer::PlayGame()
{
	is_running = true;
}

void GameTimer::PauseGame()
{
	is_paused = true; // The game is paused but is still running
	game_timer.Stop(); // We stop the game timer
}

void GameTimer::ResumeGame()
{
	is_paused = false; // The game is already running and is not longer paused
	game_timer.Resume();  // Resume the game timer
}

void GameTimer::StopGame()
{
	is_running = false; // Now is fully stopped so the game is not running
	game_timer.Stop(); // We stop the game time as well
}

float GameTimer::ReadSecGame()
{

	if (is_running && !is_paused)
	{
		return (float)(SDL_GetTicks() - game_timer.started_at) / 1000.0f;
	}

	//if (is_paused && is_running)
	//{
	//	return (float)game_timer.stopped_at;
	//}

	else
		return 0.0f;
}
