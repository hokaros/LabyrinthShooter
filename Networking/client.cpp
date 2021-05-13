#include "client.h"
//#include "connectionHandler.h"
using namespace connection;


Client::Client() {};
Client::~Client() {};

bool Client::Connect(std::string adress, uint32_t port) {

	try {

		asio::ip::tcp::resolver resolver(context);
		asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(adress, std::to_string(port));

		connectionHandler = new ConnectionHandler(endpoint, context, asio::ip::tcp::socket(context), receivedMessages);
		connectionHandler->connectToServer();

		contextThread = std::thread([this]() { context.run(); });

	}
	catch (std::exception ex) {
		std::cout << "[ERROR] An error has occurred in connection with the server";
		return false;
	}

	return true;
}


void Client::disconnect() {
	if (isConnected()) connectionHandler->disconnect();
	context.stop();
	contextThread.join();
	delete connectionHandler;

}

void Client::sendMessage(Message& message) {
	if (isConnected())
		connectionHandler->sendMessage(message);

}

bool Client::isConnected() {
	return connectionHandler == nullptr ? false : connectionHandler->isConnected();
}

