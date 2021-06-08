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

// Czas, co jaki zmienia si� labirynt
#define LAB_TIME 2

#define BULLET_BASIC_SPEED 1000
#define BULLET_BASIC_DAMAGE 1
#define BULLET_SUPER_SPEED 2000
#define BULLET_SUPER_DAMAGE 2
#define WPN_BASIC_RELOAD 0.2
#define WPN_SUPER_RELOAD 2

#define MAX_HEALTH 3
#define PLAYER_SPEED 300.0

#define PLAYER_SIGHT 400


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
	function<void(const Vector&)> onControlledDirectionChanged;
	function<void(double)> onControlledAimChanged;
	function<void()> onControlledShot;
	function<void(FirearmType)> onControlledWeaponChanged;
	function<void(bool*)> onLabChanged;

public:
	Game(Window& window, GameStartInfo&& gameInfo, bool serverVersion = false);
	~Game();

	// G��wna p�tla gry. Zwraca fa�sz, je�li w trakcie u�ytkownik zamknie okno
	bool Run();
	bool IsRunning();

	// Usuwa wszystkie obiekty
	void Clear();

	// Wywo�uje funkcj� w najbli�szej klatce
	void Invoke(function<void()> fun);

	GameObject* GetControlledPlayer();
	GameObject* GetPlayer(int id);

private:
	bool isServer;

	Window& window;
	Timer timer;
	ObjectManager objectManager;
	GameObject** players = NULL;
	size_t playerCount = 0;
	GameObject* controlledPlayer = NULL;

	GameStartInfo startInfo;
	bool isRunning = false;

	GameBitmaps bitmaps;

	BMPStats healthStats;

	GameObject basicBullet; // TODO: przenie�� prefaby do osobnej struktury
	GameObject superBullet;

	// Lista funkcji do wykonania w najbli�szej klatce
	std::list<function<void()>> invokes;

	std::mutex invokesMutex;
	std::mutex playersMutex;
	std::mutex metadataMutex;

private:
	void LoadStartingObjects();
	void SetRunning(bool running);

	GameObject* CreatePlayer(const Vector& position, bool isControlled = false);

	void InvokePostponed();

	void OnControlledDirectionChanged(const Vector& newDir);

	void Render(LabyrinthSolidifier& lab);

};