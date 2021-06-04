#include <iostream>
#include "Server.h"
#include <thread>
#include <iostream>
#include <map>

Server* server;
std::thread* serverThread;
std::map<int, std::vector<char>*> clientKeys;


void onClientConnected(int id) {
	// Dodanie klienta do listy
	clientKeys.insert(std::pair<int, std::vector<char>*>(id, new std::vector<char>()));
	std::cout << "XD\n";
	/*char* text = toCStr(id);

	SendMessageA(hClientList, LB_ADDSTRING, 0, (LPARAM)text);

	delete text;*/
}

void onClientDisconnected(int id) {
	// Usunięcie klienta z listy
	/*char* text = toCStr(id);

	int index = (int)SendMessageA(hClientList, LB_FINDSTRINGEXACT, 0, (LPARAM)text);
	SendMessageA(hClientList, LB_DELETESTRING, index, 0);

	delete text;*/
}

void onMessageReceived(int clientId, const std::string& message) {
	for (char c : message) {
		clientKeys[clientId]->push_back(c);
	}

	/*if (clientId == currentClient) {
		refreshView();
	}*/
}


void initServer() {
	server = new Server();

	server->onMessageReceived = onMessageReceived;
	server->onClientConnected = onClientConnected;
	server->onClientDisconnected = onClientDisconnected;

	serverThread = new std::thread([&]() {server->Listen(); });
}


int main()
{
    std::cout << "Hello World!\n";
	initServer();

	system("pause");

	return 0;
}