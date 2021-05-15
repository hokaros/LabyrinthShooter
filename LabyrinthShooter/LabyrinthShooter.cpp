﻿#include <iostream>
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
#include "Cage.h"
#include <list>
#include "Window.h"
#include "LabyrinthSolidifier.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define LAB_TIME 0.5

int main()
{
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!window.Init())
		return 1;

	SDL_Surface* screen = window.GetScreen();

	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xAA);

	// Input
	// Należy tu dodać wszystkie klawisze, które chce się odczytywać
	SDL_KeyCode steeringKeys[] = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_a, SDLK_w, SDLK_d, SDLK_s };
	size_t keyCount = sizeof(steeringKeys) / sizeof(SDL_KeyCode);
	InputController input(steeringKeys, keyCount);

	// Stan gry
	Timer timer;

	std::list<GameObject*> gameObjects;

	Vector mapStart(10, 10);
	LabyrinthSolidifier lab(mapStart, 10, 100, 5, 5, gameObjects, LAB_TIME);
	for (int i = 0; i < lab.WallsCount(); i++) {
		gameObjects.push_back(lab.GetWalls()[i]);
	}
	for (int i = 0; i < lab.BorderElements(); i++) {
		gameObjects.push_back(lab.GetBorder()[i]);
	}

	// TODO: pobieranie punktu wejściowego gracza
	GameObject player(Vector(20, 20), Vector(30, 250), gameObjects);
	player.AddComponent(new RectangleRenderer(player, screen, red, red));
	player.AddComponent(new PlayerController(player, 300.0f));

	gameObjects.push_back(&player);

	int quit = 0;

	timer.NextFrame();
	// Pętla gry
	while (!quit) {
		// Nowa klatka
		timer.NextFrame();

		if (!input.Update()) {
			quit = 1;
		}

		//generowanie tła
		SDL_FillRect(screen, NULL, black);

		for (GameObject* go : gameObjects) {
			go->Update();
		}
		lab.Update();

		window.Render();
	}

	return 0;
}