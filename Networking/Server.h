#pragma once
#include "connectionHandler.h"
#include <thread>

namespace connection {

	class Server {

	public:
		Server(uint16_t port);
		~Server();
		bool startSever();
		void shutDown();
		void WaitForClient();
		void MessageClients();

	private:
		asio::ip::tcp::acceptor acceptor;//to akceptuje polaczenia od klientow
		asio::io_context context;
		asio::io_context::work idleWork;
		std::thread* contextThread;
		asio::ip::tcp::endpoint endpoint;
	};
}