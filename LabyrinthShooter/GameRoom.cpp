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

		//generowanie t³a
		SDL_FillRect(screen, NULL, black);

		// Wyœwietlenie poczekalni
		window.DrawString(0, 0, "Press enter to start the game");

		{
			// Odczytanie zewnêtrznego sygna³u rozpoczêcia
			std::lock_guard<std::mutex> lock(gameStartMutex);
			shouldStart = startGame;
		}

		if (shouldStart) {
			// Wejœcie do gry
			RunGame();
			std::lock_guard<std::mutex> lock(gameStartMutex);
			startGame = false;
		}

		if (input.PressedThisFrame(SDLK_RETURN)) {
			// Wejœcie do gry
			size_t pCount = 10;
			float offset = 80;
			Vector* playerPositions = new Vector[pCount];
			for (size_t i = 0; i < pCount; i++) {
				playerPositions[i] = Vector(50 + offset * i, 50);
			}
			SetNextGame(new Game(window, 
				GameStartInfo(playerPositions, pCount))
			);
			RunGame();
		}
		else if (input.PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		window.Render();
	}
}

void GameRoom::StartGame(int selfId, Vector* playerPositions, size_t playerCount) {
	this->playerCount = playerCount;

	Vector* positionsCpy = new Vector[playerCount];
	std::memcpy(positionsCpy, playerPositions, playerCount);
	GameStartInfo gameInfo(positionsCpy, playerCount);

	std::lock_guard<std::mutex> lock(gameStartMutex);
	nextGame = new Game(window, std::move(gameInfo));
	startGame = true;
}

void GameRoom::RunGame() {
	if (nextGame == NULL)
		return;

	{
		std::lock_guard<std::mutex> lock(gameStartMutex);
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

int GameRoom::CountPlayers() const {
	return playerCount;
}

void GameRoom::SetPlayerCount(int count) {
	playerCount = count;
}

void GameRoom::SetShouldStart(bool shouldStart) {
	std::lock_guard<std::mutex> lock(gameStartMutex);

	startGame = shouldStart;
}

bool GameRoom::ShouldStart() {
	std::lock_guard<std::mutex> lock(gameStartMutex);
	return startGame;
}

void GameRoom::SetNextGame(Game* newGame) {
	std::lock_guard<std::mutex> lock(gameStartMutex);
	nextGame = newGame;
}