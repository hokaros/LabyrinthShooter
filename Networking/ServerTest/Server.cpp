#include "Server.h"

Server::Server()
	: idleWork(context),
	endpoint(asio::ip::tcp::v4(), PORT_DEFAULT), acceptor(context, endpoint),
	contextThread(NULL) {
	//	Inicjacja tablicy graczy
	for (int i = 0; i < PLAYERS_NUM; i++) players[i] = NULL;
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
			//	Dodanie do slownika powiazania id z ConnectionHandler
			clientIdMap[lastClientId] = newConnection;

			
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
	case WildMessage::PLAYER_DEATH:
		break;
	case WildMessage::WALL_DESTRUCTION:
		break;
	case WildMessage::LABIRYNTH_CHANGE:
		break;
	case WildMessage::END_OF_GAME:
		break;
	case WildMessage::POSITION:
		break;
	case WildMessage::NEW_DIRECTION:
		break;
	case WildMessage::WEAPON_CHANGE:
		break;
	case WildMessage::CHANGE_OF_AIMING_DIRECTION:
		break;
	case WildMessage::SHOT:
		break;
	case WildMessage::JOIN_REQUEST:
	{
		//	Dodanie gracza do tablicy
		ConnectionHandler<WildMessage>* connection = clientIdMap[clientId];
		bool joined = false;
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			if (players[i] == NULL)
			{
				players[i] = connection;
				joined = true;
				break;
			}
		}

		Message<WildMessage> msg;

		if (joined) msg.header.id = WildMessage::JOIN_ACCEPT;
		else msg.header.id = WildMessage::JOIN_DENIED;

		connection->WriteMessage(msg);

		break;
	}
	case WildMessage::JOIN_ACCEPT:
		break;
	case WildMessage::JOIN_DENIED:
		break;
	case WildMessage::PLAYER_JOINED:
		break;
	case WildMessage::PLAYER_LEFT:
		break;
	case WildMessage::GAME_STARTED:
		break;
	default: break;
	}
}