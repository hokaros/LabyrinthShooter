#pragma once
#include <thread>
#include "connectionHandler.h"

namespace connection {

	class Server {

	public:
		Server();
		~Server();

		void Listen();
		bool Connect(std::string adress, uint32_t port);
		void disconnect();
		void sendMessage(Message& message);
		bool isConnected();

	protected:
		connection::ConnectionHandler* connectionHandler = nullptr;
		std::queue<Message>receivedMessages;


		asio::ip::tcp::endpoint endpoint;
		asio::io_context context;
		asio::io_context::work idleWork;
		asio::ip::tcp::acceptor acceptor;
		std::thread* contextThread;

	};
}
