#include <iostream>
#include "Client.h"

Client* client;
std::thread* clientThread;

int main()
{
	std::cout << "Hello World!\n";
	client = new Client("127.0.0.1", 80);
	client->Connect();

	std::string str;
	int i, j, k;
	std::cin >> i;
	std::cin >> j;
	std::cin >> k;

	Message<WildMessage> msg;
	msg.header.id = WildMessage::WRITE;
	msg << i;
	msg << j;
	msg << k;

	client->Send(msg);

	//std::string from_msg;
	int from_msg;
	msg >> from_msg;
	std::cout << "\n" << from_msg << "\n";
	msg >> from_msg;
	std::cout << "\n" << from_msg << "\n";
	msg >> from_msg;
	std::cout << "\n" << from_msg << "\n";

	return 0;
}