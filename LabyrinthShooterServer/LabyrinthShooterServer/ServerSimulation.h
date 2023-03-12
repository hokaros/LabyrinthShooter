#pragma once
#include <Server.h>
#include <Game.h>

class ServerSimulation
{
public:
	ServerSimulation(Window* window, Server& server, GameStartInfo&& startInfo);

	void Run();

private:
	Server& server;
	Game game;

private:
	void SubscribeToGame();
	void SubscribeToServer();

	void OnDirectionChanged(int playerId, Vector newDir, Vector pos);
	void OnShot(int playerId, double dir, FirearmType wpnType, Vector pos);
	void OnWeaponChanged(int playerId, FirearmType newType);
};

