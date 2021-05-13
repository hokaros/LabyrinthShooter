#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_main.h>

class Window
{
public:
	Window(int width, int height);
	~Window();

	bool Init();
	void Render();

	SDL_Surface* GetScreen() const;

	static Window* Main();
private:
	int width;
	int height;

	SDL_Surface* screen = NULL;
	SDL_Surface* charset = NULL;
	SDL_Texture* scrtex = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	static Window* mainWindow;
};

