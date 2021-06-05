#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include "Game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main()
{
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!window.Init())
		return 1;

	Game game(window);

	game.Run();

	return 0;
}