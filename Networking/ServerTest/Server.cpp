#include "Server.h"

Server::Server()
	: idleWork(context),
	endpoint(asio::ip::tcp::v4(), PORT_DEFAULT), acceptor(context, endpoint),
	contextThread(NULL) {

}

Server::~Server() {
	Stop();
	delete contextThread;

	for (auto connection : connections) {
		delete connection;
	}
}

void Server::Listen() {
	acceptor.listen();
	contextThread = new std::thread([&]() {context.run(); });

	WaitForClientConnection();
}

void Server::WaitForClientConnection() {
	acceptor.async_accept([&](asio::error_code ec, asio::ip::tcp::socket socket) {
		if (!ec) {
			ConnectionHandler<WildMessage>* newConnection = new ConnectionHandler<WildMessage>(++lastClientId, context, std::move(socket));
			// Subskrypcja zdarzeñ
			newConnection->onMessageReceived = 
				[this](int clientId, const Message<WildMessage>& msg) {
				OnMessageReceived(clientId, msg); 
			};
			newConnection->onDisconnected = [this](int clientId) { 
				if(onClientDisconnected) 
					onClientDisconnected(clientId); 
			};

			connections.push_back(newConnection);
			if (onClientConnected)
				onClientConnected(lastClientId);

			newConnection->OpenInput();
		}

		WaitForClientConnection();
		});
}

void Server::Stop() {
	context.stop();
	if (contextThread != NULL && contextThread->joinable()) {
		contextThread->join();
		delete contextThread;
		contextThread = NULL;
	}
}

void Server::OnMessageReceived(int clientId, const Message<WildMessage>& message) {
	WildMessage msgType = message.header.id;
	Message<WildMessage> msg = message;

	switch (msgType)
	{
	case WildMessage::WRITE:
		for (int i = 0; i < 3; i++)
		{
			int from_msg;
			msg >> from_msg;
			std::cout << "\n" << from_msg << "\n";
		}
		break;
	default: break;
	}

	/*size_t size = message.body.size();
	char* str = new char[size + 1];
	std::memcpy(str, message.body.data(), size);
	str[size] = '\0';

	std::string stri;
	stri.append(str);

	onMessageReceived(clientId, stri);*/
}

void Server::RequestMouseLock(int clientId, int duration) {
	Message<WildMessage> msg;
	msg.header.id = WildMessage::LOCK_MOUSE;
	msg << duration;

	for (auto connection : connections) {
		if (connection == NULL) {
			continue;
		}

		if (!connection->IsConnected()) {
			onClientDisconnected(connection->GetId());
			continue;
		}
		if (connection->GetId() == clientId) {
			connection->WriteMessage(msg);
		}
	}
}