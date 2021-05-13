#include "Window.h"

Window* Window::mainWindow{ NULL };

Window::Window(int width, int height)
	: width(width), height(height) {
	if (mainWindow == NULL) {
		mainWindow = this;
	}
}

Window::~Window() {
	if (window != NULL) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();

	if (mainWindow == this) {
		mainWindow = NULL;
	}
}

Window* Window::Main() {
	return mainWindow;
}

bool Window::Init() {
	int rc;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	// tryb pe³noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(width, height, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return false;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, width, height);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Labyrinth Shooter");

	screen = SDL_CreateRGBSurface(0, width, height, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("resources/cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(resources/cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return false;
	};

	SDL_SetColorKey(charset, true, 0x000000);

	return true;
}

void Window::Render() {
	// Renderowanie
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	//SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

SDL_Surface* Window::GetScreen() const {
	return screen;
}