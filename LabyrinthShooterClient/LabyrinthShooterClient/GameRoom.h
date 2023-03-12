#pragma once
#include <Game.h>
#include <Draw.h>
#include <TextBox.h>
#include <mutex>
#include <client.h>

#define FONTSIZE_HEADER 25
#define FONTSIZE_MEDIUM 14

#define DEFAULT_PORT 80

class GameRoom
{
public:
	GameRoom(Window& window, Client* client);

	// G³ówna pêtla poczekalni
	void Enter();
	// Wejœcie do gry
	void StartGame(int selfId, Vector* playerPositions, size_t playerCount);

	int GetPlayerCount();
	void SetPlayerCount(int count);

private:
	Window& window;
	Game* game = NULL; // obecna gra
	Game* nextGame = NULL; // gra przygotowana do uruchomienia

	Client* client;

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

	void SubscribeToClient();
	void SubscribeToGame();

	void OnPlayerJoined();
	void OnPlayerLeft();
	void OnGameStarted(int selfId, float positions[PLAYERS_NUM][2]);
};


// Panel, w którym gracz wybiera serwer
class RoomFinder
{
public:
	RoomFinder(Window& window);
	~RoomFinder();

	void EnterSearch();
	void EnterGameRoom();

	GameRoom* GetCurrentRoom();

private:
	Window& window;
	GameRoom* currentRoom = NULL;
	Client* client = NULL;

	bool shouldEnter = false;

	std::mutex mutex;

private:
	void Draw(TextBox& textBox);

	void SetCurrentRoom(GameRoom* newRoom);
	// Konsumuj¹ce sprawdzenie sygna³u do wejœcia do pokoju
	bool ShouldEnter();

	void TryConnect(std::string ip);
	void RealEnterGameRoom();
};
