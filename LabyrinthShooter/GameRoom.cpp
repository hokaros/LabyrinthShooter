#include "GameRoom.h"

GameRoom::GameRoom(Window& window)
	: window(window), playerCount(1) {

}

void GameRoom::Enter() {
	SDL_Surface* screen = window.GetScreen();
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	InputController* input = InputController::Main();

	bool quit = false;
	bool shouldStart = false;

	while (!quit) {
		if (!input->Update()) {
			quit = 1;
		}

		// Generowanie t³a
		SDL_FillRect(screen, NULL, black);

		// Wyœwietlenie poczekalni
		DrawWaitingRoom();

		{
			// Odczytanie zewnêtrznego sygna³u rozpoczêcia
			std::lock_guard<std::mutex> lock(mutex);
			shouldStart = startGame;
		}

		if (shouldStart) {
			// Wejœcie do gry
			RunGame();
			std::lock_guard<std::mutex> lock(mutex);
			startGame = false;
		}

		if (input->PressedThisFrame(SDLK_RETURN)) {
			// Testowe wejœcie do gry
			SetNextGame(new Game(window, 
				GameStartInfo(new Vector[1]{ Vector(50, 200) }, 1, 0))
			);
			RunGame();
		}
		else if (input->PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		window.Render();
	}

	// TODO: roz³¹czenie z serwerem
}

void GameRoom::DrawWaitingRoom() {
	char buffer[16];
	// Informacja o liczbie graczy w poczekalni
	sprintf_s(buffer, "%d/%d", GetPlayerCount(), NEEDED_PLAYERS);
	window.DrawString(5, 5, buffer, FONTSIZE_MEDIUM);

	window.DrawString(window.GetWidth()/2 - 15*FONTSIZE_MEDIUM, window.GetHeight()/2 - FONTSIZE_MEDIUM, "Press enter to start the game", FONTSIZE_MEDIUM);
}

void GameRoom::StartGame(int selfId, Vector* playerPositions, size_t playerCount) {
	this->playerCount = playerCount;

	Vector* positionsCpy = new Vector[playerCount];
	std::memcpy(positionsCpy, playerPositions, playerCount);
	GameStartInfo gameInfo(positionsCpy, playerCount, selfId);

	std::lock_guard<std::mutex> lock(mutex);
	nextGame = new Game(window, std::move(gameInfo));
	startGame = true;
}

void GameRoom::RunGame() {
	if (nextGame == NULL)
		return;

	{
		std::lock_guard<std::mutex> lock(mutex);
		// Ustawienie przygotowanej gry jako aktualna
		if (game != NULL) {
			delete game;
			game = NULL;
		}
		game = nextGame;
		nextGame = NULL;
	}

	game->Run();

	delete game;
	game = NULL;
}

int GameRoom::GetPlayerCount() {
	std::lock_guard<std::mutex> lock(mutex);

	return playerCount;
}

void GameRoom::SetPlayerCount(int count) {
	std::lock_guard<std::mutex> lock(mutex);

	playerCount = count;
}

void GameRoom::SetShouldStart(bool shouldStart) {
	std::lock_guard<std::mutex> lock(mutex);

	startGame = shouldStart;
}

bool GameRoom::ShouldStart() {
	std::lock_guard<std::mutex> lock(mutex);
	return startGame;
}

void GameRoom::SetNextGame(Game* newGame) {
	std::lock_guard<std::mutex> lock(mutex);
	nextGame = newGame;
}



RoomFinder::RoomFinder(Window& window)
	: window(window) {

}

void RoomFinder::EnterSearch() {
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

		// Generowanie t³a
		SDL_FillRect(screen, NULL, black);

		textBox.Update();

		// Wyœwietlenie wyszukiwarki
		Draw(textBox);

		if (input->PressedThisFrame(SDLK_RETURN)) {
			// Testowe wejœcie do poczekalni
			// TODO: próba po³¹czenia z serwerem
			GameRoom gameRoom(window);
			gameRoom.Enter();
		}
		else if (input->PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		window.Render();
	}
}

void RoomFinder::Draw(TextBox& textBox) {
	window.DrawString(window.GetWidth() / 2 - FONTSIZE_HEADER * 9, 50, "Podaj ip serwera:", FONTSIZE_HEADER);

	textBox.Draw();
}