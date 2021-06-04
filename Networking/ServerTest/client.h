#pragma once
#include "ConnectionHandler.h"
class Client
{
public:
	void(*onMouseLocked)(int duration);
public:
	Client(const char* address, int port);
	void Connect();
	void SendKeys(char* keys, size_t count);
	void Disconnect();
	bool IsConnected();
	~Client();
private:
	asio::io_context context;
	asio::io_context::work idleWork;
	asio::ip::tcp::endpoint endpoint;
	ConnectionHandler<WildMessage>* connectionHandler;

	std::thread* contextThread;

	bool disconnected = true;

	void OnMessageReceived(const Message<WildMessage>& msg);
};

