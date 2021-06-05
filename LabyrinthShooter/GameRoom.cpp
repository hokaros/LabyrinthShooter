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

	while (!quit) {
		if (!input.Update()) {
			quit = 1;
		}

		//generowanie t³a
		SDL_FillRect(screen, NULL, black);

		// Wyœwietlenie poczekalni

		if (input.PressedThisFrame(SDLK_RETURN)) {
			// Wejœcie do gry
			Game game(window);
			game.Run();
		}
		else if (input.PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		window.Render();
	}
}

int GameRoom::CountPlayers() const {
	return playerCount;
}

void GameRoom::SetPlayerCount(int count) {
	playerCount = count;
}