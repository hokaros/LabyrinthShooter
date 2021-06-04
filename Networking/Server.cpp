#include "Server.h"
using namespace connection;

//Server::Server(uint16_t port)
	//: idleWork(context), acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {};

//Server::Server(uint16_t port)
	//: idleWork(context), acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {};
#define PORT_DEFAULT 80

Server::Server(uint16_t port)
	: idleWork(context),
	endpoint(asio::ip::tcp::v4(), PORT_DEFAULT), acceptor(context, endpoint),
	contextThread(NULL) {

}
Server::~Server() {};

bool Server::startSever() {

	try {
		acceptor.listen();
		contextThread = new std::thread([this]() { context.run(); });
		WaitForClient();		
	}
	catch (std::exception e) {

		std::cout << "[SERVER] exception during startup";
		return true;
	}
}

void Server::WaitForClient() {

	acceptor.async_accept([this](std::error_code errorCode, asio::ip::tcp::socket socket) {

		if (!errorCode) {

			std::cout << "[SERVER] New connection";
		}

		WaitForClient();
		});
}

void Server::shutDown() {
	std::cout << "shutDown";
};

void Server::MessageClients() {
	std::cout << "mes";
};