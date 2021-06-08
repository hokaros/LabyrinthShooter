#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include "ServerSimulation.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Window window(SCREEN_WIDTH, SCREEN_HEIGHT);

Server* server;
ServerSimulation* game;
std::thread* serverThread;
std::thread* gameThread;

void initServer() {
	server = new Server();

	server->onGameStarted = [&](int id, float positions[PLAYERS_NUM][2]) {

		Vector playerPositions[PLAYERS_NUM];
		for (int i = 0; i < PLAYERS_NUM; i++) {
			playerPositions[i] = Vector(positions[i][0], positions[i][1]);
		}

		game = new ServerSimulation(window, *server, GameStartInfo(playerPositions, PLAYERS_NUM, 0));
		gameThread = new std::thread([&]() {
			game->Run();
		});
	};

	serverThread = new std::thread([&]() {server->Listen(); });
}


int main()
{
	if (!window.Init())
		return 1;

	std::cout << "Server started\n";
	initServer();

	std::string command = "";

	while (command != "quit")
	{
		std::cout << "# ";
		std::cin >> command;

		if (command == "quit") break;
		else if (command == "") continue;
		else std::cout << "Nieprawidlowa komenda\n";
	}

	if (server != NULL)
		delete server;

	if (serverThread != NULL && serverThread->joinable())
		serverThread->join();

	if (gameThread != NULL && gameThread->joinable())
		gameThread->join();
	if (game != NULL)
		delete game;
	return 0;
}