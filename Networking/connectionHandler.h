#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


#include "message.h"
#include <queue>


namespace connection {


	class ConnectionHandler {

	public:

		ConnectionHandler(asio::ip::tcp::resolver::results_type& endpoint, asio::io_context& context,
			asio::ip::tcp::socket socket, std::queue<Message>& messagesIn);
		
		void connectToServer();
		bool isConnected();
		void sendMessage(Message& message);
		void writeHeader();
		void writeBody();
		void readHeader();
		void readBody();
		void saveMessage();
		void disconnect();





	protected:
		asio::io_context& context;
		asio::ip::tcp::socket socket;
		asio::ip::tcp::resolver::results_type& endpoint;
		std::queue<Message>& messagesIn;
		std::queue<Message> messagesOut;
		connection::Message tmpMessage;


	};




}
