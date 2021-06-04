#include <iostream>
#include "Client.h"

Client* client;
std::thread* clientThread;

int main()
{
	std::cout << "Hello World!\n";
	client = new Client("127.0.0.1", 80);
	client->Connect();
	return 0;
}