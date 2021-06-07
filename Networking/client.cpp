#include "Client.h"

Client::Client(const char* address, int port)
	: endpoint(asio::ip::make_address(address), port), connectionHandler(NULL),
	contextThread(NULL), idleWork(context) {
	
}

Client::~Client() {
	Disconnect();
	delete connectionHandler;
}

void Client::Connect() {
	std::error_code ec;
	contextThread = new std::thread([&]() {context.run(); });

	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint, ec);
	if (ec)
		return; // b³¹d

	connectionHandler = new ConnectionHandler<WildMessage>(0, context, std::move(socket));
	connectionHandler->onMessageReceived = [this](int conId, const Message<WildMessage>& msg) {OnMessageReceived(msg); };
	connectionHandler->OpenInput();

	disconnected = false;
}

void Client::SendKeys(char* keys, size_t count) {
	if (connectionHandler == NULL)
		return;
	
	Message<WildMessage> message;
	message.header.id = WildMessage::KEYS;

	for (size_t i = 0; i < count; i++) {
		message << keys[i];
	}
	connectionHandler->WriteMessage(message);
}

void Client::Disconnect() {
	context.stop();
	if (contextThread != NULL && contextThread->joinable()) {
		contextThread->join();
		delete contextThread;
		contextThread = NULL;
	}
	if (connectionHandler != NULL) {
		delete connectionHandler;
		connectionHandler = NULL;
	}

	disconnected = true;
}

bool Client::IsConnected() {
	return !disconnected;
}

bool Client::Send(const Message<WildMessage>& msg)
{
	if (connectionHandler == NULL) {
		return false;
	}

	if (!connectionHandler->IsConnected()) {
		//onClientDisconnected(connection->GetId());
		return false;
	}
	connectionHandler->WriteMessage(msg);
	return true;
}

void Client::GameProtocol()
{
	Message<WildMessage> msg;
	msg.header.id = WildMessage::JOIN_REQUEST;

	Send(msg);
}


void Client::OnMessageReceived(const Message<WildMessage>& message) {
	WildMessage msgType = message.header.id;
	Message<WildMessage> msg = message;

	int id;

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
	case WildMessage::WALL_DESTRUCTION:
		break;
	case WildMessage::LABIRYNTH_CHANGE:
		break;
	case WildMessage::END_OF_GAME:
		break;
	case WildMessage::POSITION:
		break;
	case WildMessage::PLAYER_DEATH:
		msg >> id;

		if (onPlayerDead)
			onPlayerDead(id);
		break;
	case WildMessage::NEW_DIRECTION: {
		// Odebranie kierunku, w którym zacz¹³ siê poruszaæ gracz i jego id
		Vector dir;
		msg >> id >> dir;

		if (onDirectionChanged)
			onDirectionChanged(id, dir);
		break;
	}
	case WildMessage::WEAPON_CHANGE: {
		FirearmType newType;

		msg >> id >> newType;

		if (onWeaponChanged)
			onWeaponChanged(id, newType);
		break;
	}
	case WildMessage::CHANGE_OF_AIMING_DIRECTION: {
		double rotation;
		msg >> id >> rotation;

		if (onAimChanged)
			onAimChanged(id, rotation);
		break;
	}
	case WildMessage::SHOT:
		msg >> id;

		if (onShot)
			onShot(id);
		break;
	case WildMessage::JOIN_REQUEST:
		break;
	case WildMessage::JOIN_ACCEPT:
		std::cout << "Joined\n";

		if (onJoinAccepted)
			onJoinAccepted();
		break;
	case WildMessage::JOIN_DENIED:
		std::cout << "Waiting\n";

		if (onJoinDenied)
			onJoinDenied();
		break;
	case WildMessage::PLAYER_JOINED:
		if (onPlayerJoined)
			onPlayerJoined();
		break;
	case WildMessage::PLAYER_LEFT:
		if (onPlayerLeft)
			onPlayerLeft();
		break;
	case WildMessage::GAME_STARTED:
	{
		std::cout << "Game Started\n";
		int id;
		float positions[PLAYERS_NUM][2];

		//	TODO
		int num = 4;

		msg >> id;
		std::cout << "Id: " << id << "\n";
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			msg >> positions[i][0];
			msg >> positions[i][1];
			std::cout << "Position " << i << ": " << positions[i][0] << ' ' << positions[i][1] << "\n";
		}

		std::cout << "Walls : " ;
		bool* walls = new bool[num];
		for (int i = 0; i < num; i++)
		{
			bool wall;
			msg >> wall;
			std::cout << wall << ' ';

			walls[i] = wall;
		}
		std::cout << "\n";
		//

		if (onGameStarted)
			onGameStarted(id, positions, walls);
		break;
	}
	default: break;
	}
}
//Zmiany
Message<WildMessage> Client::CreateMessagePlayerDeath(int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::PLAYER_DEATH;
	message << id;
	return message;
};
Message<WildMessage> Client::CreateMessageWallDestruction(int x, int y) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WALL_DESTRUCTION;
	message << x;
	message << y;
	return message;
};
Message<WildMessage> Client::CreateMessageLabirynthChange(bool* change, int size) {

	Message<WildMessage> message;
	message.header.id = WildMessage::LABIRYNTH_CHANGE;
	for (int i = 0; i < size; i++) {
		message << change[i];
	}
	return message;
};
/*Message<WildMessage> Client::CreateMessagePosition(Vector pos, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::POSITION;
	message << id;
	message << pos;
	return message;
};*/
Message<WildMessage> Client::CreateMessageNewDirection(Vector direction) {

	Message<WildMessage> message;
	message.header.id = WildMessage::NEW_DIRECTION;
	message << direction;
	return message;
};
Message<WildMessage> Client::CreateMessageWeaponChange(FirearmType type) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WEAPON_CHANGE;
	message << type;
	return message;
};
Message<WildMessage> Client::CreateMessageChangeOfAimingDirection(double aimDir) {

	Message<WildMessage> message;
	message.header.id = WildMessage::CHANGE_OF_AIMING_DIRECTION;
	message << aimDir;
	return message;
};
Message<WildMessage> Client::CreateMessageJoinRequest() {

	Message<WildMessage> message;
	message.header.id = WildMessage::JOIN_REQUEST;
	return message;
};
Message<WildMessage> Client::CreateMessagePlayerShot() {

	Message<WildMessage> message;
	message.header.id = WildMessage::SHOT;
	return message;
}