#include <iostream>
#include "Server.h"

using namespace connection;

int main()
{
	std::cout << "Hello World!\n";

	//Client* client = new Client();
	Server* server = new Server();
	server->Listen();


	return 0;
}