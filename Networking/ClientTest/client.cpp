#include "Client.h"

Client::Client(const char* address, int port)
	: endpoint(asio::ip::make_address(address), port), connectionHandler(NULL),
	contextThread(NULL), idleWork(context), onMouseLocked(NULL) {
	
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

bool Client::Send(Message<WildMessage>& msg)
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


void Client::OnMessageReceived(const Message<WildMessage>& msg) {
	if (msg.header.id == WildMessage::LOCK_MOUSE) {
		int duration = 0;
		std::memcpy(&duration, msg.body.data(), (sizeof(int) <= msg.body.size()) ? sizeof(int) : msg.body.size());

		if (onMouseLocked)
			onMouseLocked(duration);
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
Message<WildMessage> Client::CreateMessagePosition(Vector pos, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::POSITION;
	message << id;
	message << pos;
	return message;
};
Message<WildMessage> Client::CreateMessageNewDirection(Vector direction, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::NEW_DIRECTION;
	message << id;
	message << direction;
	return message;
};
Message<WildMessage> Client::CreateMessageWeaponChange(WeaponType type, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WEAPON_CHANGE;
	message << id;
	message << type;
	return message;
};
Message<WildMessage> Client::CreateMessageChangeOfAimingDirection(float aimDir, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::CHANGE_OF_AIMING_DIRECTION;
	message << id;
	message << aimDir;
	return message;
};
Message<WildMessage> Client::CreateMessageJoinRequest() {

	Message<WildMessage> message;
	message.header.id = WildMessage::JOIN_REQUEST;
	return message;
};