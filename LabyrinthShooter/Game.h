#pragma once
#include <list>

#include "Timer.h"
#include "Draw.h"
#include "InputController.h"
#include "Vector.h"
#include "GameObject.h"
#include "RectangleRenderer.h"
#include "SpriteRenderer.h"
#include "PlayerController.h"
#include "Cage.h"
#include "Window.h"
#include "LabyrinthSolidifier.h"
#include "ObjectManager.h"
#include "PowerBullet.h"
#include "PlayerEquipment.h"
#include "Firearm.h"
#include "Health.h"

#define LAB_TIME 0.5

#define BULLET_BASIC_SPEED 1000
#define BULLET_BASIC_DAMAGE 1
#define BULLET_SUPER_SPEED 2000
#define BULLET_SUPER_DAMAGE 2
#define WPN_BASIC_RELOAD 0.2
#define WPN_SUPER_RELOAD 2

#define MAX_HEALTH 3


struct GameBitmaps {
public:
	SDL_Surface* playerBmp;
	SDL_Surface* wpnBasicBmp;
	SDL_Surface* wpnSuperBmp;

public:
	GameBitmaps();
	~GameBitmaps();

	bool IsOk() const;

private:
	bool bitmapsOk = true;
};

class Game
{
public:
	Game(Window& window);

	// G³ówna pêtla gry
	void Run();

	// Usuwa wszystkie obiekty
	void Clear();

private:
	Window& window;
	Timer timer;
	InputController input;
	ObjectManager objectManager;

	GameBitmaps bitmaps;

	GameObject basicBullet; // TODO: przenieœæ prefaby do osobnej struktury
	GameObject superBullet;

private:
	void LoadStartingObjects();
};