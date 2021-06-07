#pragma once
#include "ConnectionHandler.h"
#include "Message.h"
#include "../LabyrinthShooter/Shared.h"
#include "../LabyrinthShooter/Vector.h"

#define PLAYERS_NUM 2

using std::function;


class Client
{
public:
	function<void()> onJoinAccepted;
	function<void()> onJoinDenied;
	function<void()> onPlayerJoined;
	function<void()> onPlayerLeft;
	function<void(int id, float positions[PLAYERS_NUM][2], bool* walls)> onGameStarted;
	function<void(int id, Vector newDir)> onDirectionChanged;
	function<void(int id, FirearmType newType)> onWeaponChanged;
	function<void(int id)> onPlayerDead;
	function<void(int id, double rotation)> onAimChanged;
	function<void(int id)> onShot;
public:
	Client(const char* address, int port);
	void Connect();
	void SendKeys(char* keys, size_t count);
	void Disconnect();
	bool IsConnected();
	bool Send(const Message<WildMessage>& msg);
	void GameProtocol();
	static Message<WildMessage> CreateMessagePlayerDeath(int id);
	static Message<WildMessage> CreateMessageWallDestruction(int x, int y);
	static Message<WildMessage> CreateMessageLabirynthChange(bool* change, int size);
	//Message<WildMessage> CreateMessagePosition(Vector pos, int id);
	static Message<WildMessage> CreateMessageNewDirection(Vector direction);
	static Message<WildMessage> CreateMessageWeaponChange(FirearmType type);
	static Message<WildMessage> CreateMessageChangeOfAimingDirection(double aimDir);
	static Message<WildMessage> CreateMessageJoinRequest();
	static Message<WildMessage> CreateMessagePlayerShot();
	~Client();
protected:
	virtual void OnMessageReceived(const Message<WildMessage>& msg);

private:
	asio::io_context context;
	asio::io_context::work idleWork;
	asio::ip::tcp::endpoint endpoint;
	ConnectionHandler<WildMessage>* connectionHandler;

	std::thread* contextThread;

	bool disconnected = true;

};

