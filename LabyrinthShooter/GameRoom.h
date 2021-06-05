#pragma once
#include "Game.h"

class GameRoom
{
public:
	GameRoom(Window& window);

	// G��wna p�tla poczekalni
	void Enter();

	int CountPlayers() const;
	void SetPlayerCount(int count);

private:
	Window& window;
	InputController input;

	int playerCount;
};

