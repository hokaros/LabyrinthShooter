#include "Server.h"

connection::Server::Server()
	: idleWork(context),
	endpoint(asio::ip::tcp::v4(), 80), acceptor(context, endpoint),
	contextThread(NULL) {

}

void connection::Server::Listen() {
	acceptor.listen();
	contextThread = new std::thread([&]() {context.run(); });

}