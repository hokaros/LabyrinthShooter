#pragma once
#include "Game.h"
#include "Draw.h"
#include <mutex>

class GameRoom
{
public:
	GameRoom(Window& window);

	// G��wna p�tla poczekalni
	void Enter();
	// Wej�cie do gry
	void StartGame(int selfId, Vector* playerPositions, size_t playerCount);

	int CountPlayers() const;
	void SetPlayerCount(int count);

private:
	Window& window;
	InputController input;
	Game* game; // obecna gra
	Game* nextGame; // gra przygotowana do uruchomienia

	int playerCount;

	bool startGame = false; // Sygna� do rozpocz�cia gry
	std::mutex gameStartMutex;

private:
	// Uruchamia p�tl� gry
	void RunGame();

	void SetShouldStart(bool shouldStart);
	bool ShouldStart();

	void SetNextGame(Game* newGame);
};

