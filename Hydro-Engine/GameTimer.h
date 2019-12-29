#pragma once

#include "Timer.h"

class GameTimer
{
public:
	GameTimer();
	~GameTimer();
	void Start();
	void PlayGame();
	void PauseGame();
	void ResumeGame();
	void StopGame();
	float ReadSecGame();

public:
	Timer game_timer;
	float delta_time; //  last frame time expressed in seconds
	float time_scale; // scale at which time is passing
	float real_time_since_startup;
	float real_time_delta_time;
	float time; // seconds since game start

	bool is_running = false;
	bool is_paused = false;
	bool we_stoped = false;
};
