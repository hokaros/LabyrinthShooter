#pragma once
#include "GameRoom.h"

// Panel, w kt�rym gracz wybiera serwer
class GameRoomFinder
{
public:
	GameRoomFinder(Window& window);
	~GameRoomFinder();

	void SearchLoop();
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
	TextBox CreateIpTextBox();

	void SetCurrentRoom(GameRoom* newRoom);
	// Konsumuj�ce sprawdzenie sygna�u do wej�cia do pokoju
	bool ShouldEnter();

	void TryConnect(std::string ip);
	void RealEnterGameRoom();
};
