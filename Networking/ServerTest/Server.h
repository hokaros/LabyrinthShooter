#pragma once
#include <thread>
#include <deque>

#include "../connection/ConnectionHandler.h"
#include "../connection/Message.h"

#define PORT_DEFAULT 80


class Server
{
public:
	// Zdarzenia
	void (*onClientConnected)(int id);
	void (*onClientDisconnected)(int id);
	void (*onMessageReceived) (int clientId, const std::string& msg);

	Server();
	void Listen();
	void RequestMouseLock(int clientId, int duration);
	void Stop();
	~Server();
protected:
	void OnMessageReceived(int clientId, const Message<WildMessage>& message);
private:
	int lastClientId = 0;

	asio::ip::tcp::endpoint endpoint;
	asio::io_context context;
	asio::io_context::work idleWork;
	asio::ip::tcp::acceptor acceptor;

	std::deque<ConnectionHandler<WildMessage>*> connections;
	std::thread* contextThread;
private:
	void WaitForClientConnection();
};

