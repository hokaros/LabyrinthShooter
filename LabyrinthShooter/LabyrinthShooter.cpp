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
#include "SpriteRenderer.h"
#include "PlayerController.h"
#include "Cage.h"
#include <list>
#include "Window.h"
#include "LabyrinthSolidifier.h"
#include "ObjectManager.h"
#include "PowerBullet.h"
#include "PlayerEquipment.h"
#include "Firearm.h"
#include "Health.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define LAB_TIME 0.5

#define BULLET_BASIC_SPEED 1000
#define BULLET_SUPER_SPEED 2000
#define WPN_BASIC_RELOAD 0.2
#define WPN_SUPER_RELOAD 2

#define MAX_HEALTH 3

int main()
{
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!window.Init())
		return 1;

	SDL_Surface* screen = window.GetScreen();
	SDL_Surface* playerBmp = SDL_LoadBMP("resources/player.bmp");
	if (playerBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/player.bmp\n");
		return 1;
	}
	SDL_Surface* wpnPrimaryBmp = SDL_LoadBMP("resources/weapon_primary.bmp");
	if (wpnPrimaryBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/weapon_primary.bmp\n");
		return 1;
	}
	SDL_Surface* wpnSuperBmp = SDL_LoadBMP("resources/weapon_super.bmp");
	if (wpnSuperBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/weapon_super.bmp\n");
		return 1;
	}

	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xAA);
	int yellow = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);

	// Input
	// Należy tu dodać wszystkie klawisze, które chce się odczytywać
	SDL_KeyCode steeringKeys[] = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_a, SDLK_w, SDLK_d, SDLK_s,
		SHOOT_KEY,
		WPN_SWITCH_KEY
	};
	size_t keyCount = sizeof(steeringKeys) / sizeof(SDL_KeyCode);
	InputController input(steeringKeys, keyCount);

	// Stan gry
	Timer timer;

	ObjectManager objectManager;

	Vector mapStart(10, 10);
	LabyrinthSolidifier lab(mapStart, 10, 100, 5, 5, objectManager.GetAllObjects(), LAB_TIME);
	for (int i = 0; i < lab.WallsCount(); i++) {
		objectManager.AddUndestroyable(lab.GetWalls()[i]);
	}
	for (int i = 0; i < lab.BorderElements(); i++) {
		objectManager.AddUndestroyable(lab.GetBorder()[i]);
	}

	// TODO: pobieranie pozycji wejściowej gracza
	GameObject* player = new GameObject(Vector(20, 20), Vector(30, 250), objectManager.GetAllObjects());
	player->AddComponent(new SpriteRenderer(*player, screen, playerBmp));
	player->AddComponent(new PlayerController(*player, 300.0f));
	objectManager.AddObject(player);

	// Prefab zwykłego pocisku
	GameObject basicBullet(
		Vector(4, 4),
		objectManager.GetAllObjects()
	);
	basicBullet.AddComponent(new Bullet(basicBullet, BULLET_BASIC_SPEED));
	basicBullet.AddComponent(new RectangleRenderer(basicBullet, screen, yellow, yellow));

	// Prefab silnego pocisku
	GameObject superBullet(
		Vector(10, 10),
		objectManager.GetAllObjects()
	);
	superBullet.AddComponent(new PowerBullet(superBullet, BULLET_SUPER_SPEED));
	superBullet.AddComponent(new RectangleRenderer(superBullet, screen, red, red));

	// Broń
	GameObject* basicWeapon = new GameObject(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x,
		objectManager.GetAllObjects()
	);
	basicWeapon->AddComponent(new Firearm(*basicWeapon, basicBullet, WPN_BASIC_RELOAD));
	basicWeapon->AddComponent(new SpriteRenderer(*basicWeapon, screen, wpnPrimaryBmp));
	player->AddChild(basicWeapon);
	objectManager.AddObject(basicWeapon);
	// Silna broń
	GameObject* superWeapon = new GameObject(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x,
		objectManager.GetAllObjects()
	);
	superWeapon->AddComponent(new Firearm(*superWeapon, superBullet, WPN_SUPER_RELOAD));
	superWeapon->AddComponent(new SpriteRenderer(*superWeapon, screen, wpnSuperBmp));
	player->AddChild(superWeapon);
	objectManager.AddObject(superWeapon);

	// Ekwipunek
	player->AddComponent(new PlayerEquipment(*player, basicWeapon, superWeapon));
	// Zdrowie
	Health* playerHealth = new Health(*player, MAX_HEALTH);
	player->AddComponent(playerHealth);
	playerHealth->SubscribeDeath(
		[&](Health* deadPlayer) {
			printf("Dead\n");
			objectManager.DestroyObject(&(deadPlayer->GetOwner()));
		}
	);



	int quit = 0;

	timer.NextFrame();

	for (GameObject* go : objectManager.GetAllObjects()) {
		go->Start();
	}

	// Pętla gry
	while (!quit) {
		// Nowa klatka
		timer.NextFrame();

		if (!input.Update()) {
			quit = 1;
		}

		//generowanie tła
		SDL_FillRect(screen, NULL, black);

		for (GameObject* go : objectManager.GetAllObjects()) {
			go->Update();
		}
		lab.Update();

		window.Render();

		objectManager.DisposeDestroyed();
	}

	SDL_FreeSurface(playerBmp);

	return 0;
}