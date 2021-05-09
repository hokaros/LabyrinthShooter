#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include "Timer.h"
#include "Draw.h"
#include "InputController.h"
#include "Vector.h"
#include "GameObject.h"
#include "RectangleRenderer.h"
#include "PlayerController.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main()
{
	int rc;
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pełnoekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Labyrinth Shooter");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("resources/cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(resources/cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	SDL_SetColorKey(charset, true, 0x000000);

	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);

	// Stan gry
	GameObject player(Vector(100.0f, 100.0f));
	player.AddComponent(new RectangleRenderer(player, screen, red, red));
	player.AddComponent(new PlayerController(player, 100.0f));

	int quit = 0;
	SDL_KeyCode steeringKeys[] = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_a, SDLK_w, SDLK_d, SDLK_s };
	size_t keyCount = sizeof(steeringKeys) / sizeof(SDL_KeyCode);
	InputController input(steeringKeys, keyCount);
	Timer timer;

	double totalTime = 0.0;

	// Pętla gry
	while (!quit) {
		// Nowa klatka
		timer.NextFrame();
		totalTime += timer.GetDeltaTime();

		if (!input.Update()) {
			quit = 1;
		}

		//generowanie tła
		SDL_FillRect(screen, NULL, black);

		player.Update();

		// Renderowanie
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}