#include <iostream>
#include "client.h"

//using namespace connection;

int main()
{
    std::cout << "Hello World!\n";

	//Client* client = new Client();
	connection::Client client;
	client.Connect("51.38.81.49", 80);
	system("pause");

	return 0;
}