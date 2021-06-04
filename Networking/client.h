#pragma once
#include "connectionHandler.h"

namespace connection {
	
	class Client {

	public:
		Client();
		~Client();

		bool Connect(std::string adress, uint32_t port);
		void disconnect();
		void sendMessage(Message& message);
		bool isConnected();

	protected:
		connection::ConnectionHandler* connectionHandler = nullptr;
		std::queue<Message>receivedMessages;
		std::thread contextThread;
		asio::io_context context;

	};
}