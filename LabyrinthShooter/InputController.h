#pragma once
#include <iostream>
#include <SDL.h>

class InputController
{
public:
	InputController(SDL_KeyCode* managedKeys, size_t keysCount);
	~InputController();
	// Aktualizuje statusy klawiszy. Zwraca false, je�li zamkni�to okno
	bool Update();
	bool IsKeyDown(SDL_KeyCode key) const;

	static InputController* Main();
private:
	size_t keysCount;
	SDL_KeyCode* managedKeys;
	bool* keyDownInfo;

	static InputController* main;
};

