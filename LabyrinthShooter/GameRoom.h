#pragma once
#include "Game.h"
#include "Draw.h"
#include <mutex>

#define WAITING_FONTSIZE 14

#define NEEDED_PLAYERS 4

class GameRoom
{
public:
	GameRoom(Window& window);

	// G³ówna pêtla poczekalni
	void Enter();
	// Wejœcie do gry
	void StartGame(int selfId, Vector* playerPositions, size_t playerCount);

	int GetPlayerCount();
	void SetPlayerCount(int count);

private:
	Window& window;
	InputController input;
	Game* game = NULL; // obecna gra
	Game* nextGame = NULL; // gra przygotowana do uruchomienia

	int playerCount;

	bool startGame = false; // Sygna³ do rozpoczêcia gry
	std::mutex mutex;

private:
	void DrawWaitingRoom();

	// Uruchamia pêtlê gry
	void RunGame();

	void SetShouldStart(bool shouldStart);
	bool ShouldStart();

	void SetNextGame(Game* newGame);
};

