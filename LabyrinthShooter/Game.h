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
#define LAB_X 7
#define LAB_Y 5
#define WALL_THICKNESS 10
#define WALL_LENGTH 100

#define BULLET_BASIC_SPEED 1000
#define BULLET_BASIC_DAMAGE 1
#define BULLET_SUPER_SPEED 2000
#define BULLET_SUPER_DAMAGE 2
#define WPN_BASIC_RELOAD 0.2
#define WPN_SUPER_RELOAD 2

#define MAX_HEALTH 3
#define PLAYER_SPEED 300.0


struct GameBitmaps {
public:
	SDL_Surface* playerBmp;
	SDL_Surface* wpnBasicBmp;
	SDL_Surface* wpnSuperBmp;
	SDL_Surface* heartBmp;

public:
	GameBitmaps();
	~GameBitmaps();

	bool IsOk() const;

private:
	bool bitmapsOk = true;
};


struct GameStartInfo {
public:
	GameStartInfo(Vector* playerPositions, size_t playerCount, int controllableIndex);
	GameStartInfo(GameStartInfo&& other);
	~GameStartInfo();

	Vector GetPlayerPosition(int index) const;
	size_t GetPlayerCount() const;
	int GetControllableIndex() const;

private:
	Vector* playerPositions;
	size_t playerCount;
	int controllableIndex;
};


class Game
{
public:
	Game(Window& window, GameStartInfo&& gameInfo);

	// G³ówna pêtla gry. Zwraca fa³sz, jeœli w trakcie u¿ytkownik zamknie okno
	bool Run();

	// Usuwa wszystkie obiekty
	void Clear();

private:
	Window& window;
	Timer timer;
	ObjectManager objectManager;

	GameStartInfo startInfo;

	GameBitmaps bitmaps;

	BMPStats healthStats;

	GameObject basicBullet; // TODO: przenieœæ prefaby do osobnej struktury
	GameObject superBullet;

private:
	void LoadStartingObjects();

	GameObject* CreatePlayer(const Vector& position, bool isControlled = false);
};