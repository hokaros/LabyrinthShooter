#include "GameRoom.h"


// Input
// Nale¿y tu dodaæ wszystkie klawisze, które chce siê odczytywaæ podczas gry i menu
SDL_KeyCode steeringKeys[] = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_a, SDLK_w, SDLK_d, SDLK_s,
	SHOOT_KEY,
	WPN_SWITCH_KEY,
	SDLK_ESCAPE,
	SDLK_RETURN
};

GameRoom::GameRoom(Window& window)
	: window(window), playerCount(1),
	input(steeringKeys, sizeof(steeringKeys) / sizeof(SDL_KeyCode)) {

}

void GameRoom::Enter() {
	SDL_Surface* screen = window.GetScreen();
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	bool quit = false;
	bool shouldStart = false;

	while (!quit) {
		if (!input.Update()) {
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

		if (input.PressedThisFrame(SDLK_RETURN)) {
			// Testowe wejœcie do gry
			SetNextGame(new Game(window, 
				GameStartInfo(new Vector[1]{ Vector(50, 200) }, 1, 0))
			);
			RunGame();
		}
		else if (input.PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		window.Render();
	}
}

void GameRoom::DrawWaitingRoom() {
	char buffer[16];
	// Informacja o liczbie graczy w poczekalni
	sprintf_s(buffer, "%d/%d", GetPlayerCount(), NEEDED_PLAYERS);
	window.DrawString(5, 5, buffer, WAITING_FONTSIZE);

	window.DrawString(window.GetWidth()/2 - 15*WAITING_FONTSIZE, window.GetHeight()/2 - WAITING_FONTSIZE, "Press enter to start the game", WAITING_FONTSIZE);
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