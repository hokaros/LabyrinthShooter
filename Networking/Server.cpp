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
			// Subskrypcja zdarzeń
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
			bool from_msg;
			msg >> from_msg;
			std::cout << from_msg << "\n";
		}
		break;
	case WildMessage::WALL_DESTRUCTION:
		break;
	case WildMessage::LABIRYNTH_CHANGE:
		break;
	case WildMessage::END_OF_GAME:
		break;
	case WildMessage::POSITION:
		break;

	case WildMessage::NEW_DIRECTION: {
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozesłanie
		MessageAllClients(msg);
		// Przetworzenie
		ReceiveMessageNewDirection(msg);
		break;
	}
	case WildMessage::WEAPON_CHANGE: {
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozesłanie
		MessageAllClients(msg);
		// Przetworzenie
		ReceiveMessageWeaponChange(msg);
		break;
	}
	case WildMessage::CHANGE_OF_AIMING_DIRECTION: {
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozesłanie
		MessageAllClients(msg);
		// Przetworzenie
		ReceiveMessageAimChange(msg);
		break;
	}
	case WildMessage::SHOT: {
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozesłanie
		MessageAllClients(msg);
		// Przetworzenie
		ReceiveMessageShot(msg);
		break;
	}
	case WildMessage::PLAYER_DEATH:
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozesłanie
		MessageAllClients(msg);
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
				clientIdIndexMap[clientId] = i;
				joined = true;
				break;
			}
		}

		Message<WildMessage> msg;

		if (joined) msg.header.id = WildMessage::JOIN_ACCEPT;
		else msg.header.id = WildMessage::JOIN_DENIED;

		connection->WriteMessage(msg);

		if (playersReady()) initGame();

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

bool Server::playersReady()
{
	bool ready = true;
	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		if (players[i] == NULL)
		{
			return false;
		}
	}
	return true;
}

void Server::initGame()
{
	GenerateAndSendPositions();
	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		Message<WildMessage> message = CreateMessageGameInit(i);
		players[i]->WriteMessage(message);

		if (i == 0)
			ReceiveMessageGameStarted(message); // przetworzenie przez serwer
	}
}

void Server::GenerateAndSendPositions() {

	srand((unsigned int)time(NULL));
	const float offsetX = 50;
	const float offsetY = 50;

	float tmpX;
	float tmpY;


	for (int i = 0; i < PLAYERS_NUM; i++) {

		while (true) {

			bool inValidPosition = false;
			tmpX = (float(std::rand()) / float((RAND_MAX)) * WIDTH) + MAP_START_X;
			tmpY = (float(std::rand()) / float((RAND_MAX)) * HEIGHT) + MAP_START_Y;

			for (int j = 0; j < posX.size(); j++) {
				if (abs(posX[j] - tmpX) < offsetX && abs(posY[j] - tmpY) < offsetY) inValidPosition = true;
			}

			if (!inValidPosition) {
				posY.push_back(tmpY);
				posX.push_back(tmpX);
				break;
			}
		}
	}

	/*Message<WildMessage> msg;
	msg.header.id = WildMessage::POSITION;
	for (int i = PLAYERS_NUM - 1; i >= 0; i--) {
		msg << posY[i];
		msg << posX[i];
	}
	MessageAllClients(msg);*/
}

void Server::MessageAllClients(const Message<WildMessage>& message) {

	for (int i = 0; i < PLAYERS_NUM; i++) {

		players[i]->WriteMessage(message);
	}
}

//	Creating message functions
Message<WildMessage> Server::CreateMessageGameInit(int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::GAME_STARTED;
	//	TODO


	for (int i = PLAYERS_NUM - 1; i >= 0; i--) {
		message << posY[i];
		message << posX[i];
	}

	message << id;
	//
	return message;
};

Message<WildMessage> Server::CreateMessagePlayerDeath(int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::PLAYER_DEATH;
	message << id;
	return message;
};

Message<WildMessage> Server::CreateMessageLabirynthChange(bool* newWalls) {
	Message<WildMessage> message;
	message.header.id = WildMessage::LABIRYNTH_CHANGE;

	int wallsNum = Labirynt::MemorySize(LAB_X, LAB_Y);

	for (int i = wallsNum - 1; i >= 0; i--) {
		message << newWalls[i];
	}

	return message;
}

Message<WildMessage> Server::CreateMessagePlayerHurt(int id, int dmg) {
	Message<WildMessage> message;
	message.header.id = WildMessage::PLAYER_HURT;

	message << dmg << id;

	return message;
}
