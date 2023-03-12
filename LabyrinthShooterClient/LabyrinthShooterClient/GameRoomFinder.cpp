#include "GameRoomFinder.h"

GameRoomFinder::GameRoomFinder(Window& window)
	: window(window) {

}

GameRoomFinder::~GameRoomFinder() {
	if (client != NULL) {
		delete client;
	}
}

void GameRoomFinder::EnterSearch() {
	SDL_Surface* screen = window.GetScreen();
	int white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	SDL_Rect textArea;
	textArea.x = 50;
	textArea.y = window.GetHeight() / 2;
	textArea.w = window.GetWidth() - 2 * 50;
	textArea.h = 50;
	TextBox textBox(textArea, white, black, 20);


	InputController* input = InputController::Main();

	bool quit = false;

	while (!quit) {

		if (!input->Update()) {
			quit = 1;
		}

		if (input->PressedThisFrame(SDLK_RETURN)) {
			// Testowa próba po³¹czenia z serwerem
			TryConnect(textBox.GetContent());
		}
		else if (input->PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		// Generowanie t³a
		SDL_FillRect(screen, NULL, black);

		textBox.Update();

		// Wyœwietlenie wyszukiwarki
		Draw(textBox);

		window.Render();


		if (ShouldEnter()) {
			RealEnterGameRoom();
		}
	}
}

void GameRoomFinder::EnterGameRoom() {
	std::lock_guard<std::mutex> lock(mutex);

	shouldEnter = true;
}

void GameRoomFinder::RealEnterGameRoom() {
	GameRoom gameRoom(window, client);
	SetCurrentRoom(&gameRoom);

	gameRoom.Enter();

	// Po wyjœciu z pokoju
	client->Disconnect();
	SetCurrentRoom(NULL);
}

GameRoom* GameRoomFinder::GetCurrentRoom() {
	std::lock_guard<std::mutex> lock(mutex);

	return currentRoom;
}

void GameRoomFinder::SetCurrentRoom(GameRoom* newRoom) {
	std::lock_guard<std::mutex> lock(mutex);

	currentRoom = newRoom;
}

void GameRoomFinder::TryConnect(std::string ip) {
	if (client != NULL)
		delete client;

	client = new Client(ip.c_str(), DEFAULT_PORT);
	client->onJoinAccepted = [this]() {EnterGameRoom(); };
	client->Connect();
	client->GameProtocol();
}

bool GameRoomFinder::ShouldEnter() {
	std::lock_guard<std::mutex> lock(mutex);

	bool should = shouldEnter;
	shouldEnter = false;
	return should;
}

void GameRoomFinder::Draw(TextBox& textBox) {
	window.DrawString(window.GetWidth() / 2 - FONTSIZE_HEADER * 9, 50, "Podaj ip serwera:", FONTSIZE_HEADER);

	textBox.Draw();
}