#include "Game.h"

Game::Game(Window& window, GameStartInfo&& gameInfo)
	: window(window),
	basicBullet(Vector(4, 4), objectManager.GetAllObjects()), // uwa¿aæ przy zmienianiu objectManagera
	superBullet(Vector(10, 10), objectManager.GetAllObjects()),
	startInfo(std::move(gameInfo)),
	healthStats(bitmaps.heartBmp, VectorInt(30, 30), VectorInt(3, 3)) {

	SDL_Surface* screen = window.GetScreen();

	int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int yellow = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);

	basicBullet.AddComponent(new Bullet(basicBullet, BULLET_BASIC_SPEED, BULLET_BASIC_DAMAGE));
	basicBullet.AddComponent(new RectangleRenderer(basicBullet, screen, yellow, yellow));

	superBullet.AddComponent(new PowerBullet(superBullet, BULLET_SUPER_SPEED, BULLET_SUPER_DAMAGE));
	superBullet.AddComponent(new RectangleRenderer(superBullet, screen, red, red));
}

Game::~Game() {
	if (players != NULL) {
		delete[] players; // usuniêcie tablicy wskaŸników
	}
}

void Game::LoadStartingObjects() {
	// Za³adowanie graczy
	playerCount = startInfo.GetPlayerCount();
	players = new GameObject * [playerCount];

	for (size_t i = 0; i < playerCount; i++) {
		bool isControllable = startInfo.GetControllableIndex() == i;
		GameObject* newPlayer = CreatePlayer(startInfo.GetPlayerPosition(i), isControllable);

		players[i] = newPlayer;
		if (isControllable) {
			controlledPlayer = newPlayer;
		}
	}
}

bool Game::Run() {
	InputController* input = InputController::Main();

	SDL_Surface* screen = window.GetScreen();
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	Vector mapStart(10, 10);
	LabyrinthSolidifier lab(mapStart, WALL_THICKNESS, WALL_LENGTH, LAB_X, LAB_Y, objectManager.GetAllObjects(), LAB_TIME);
	for (int i = 0; i < lab.WallsCount(); i++) {
		objectManager.AddUndestroyable(lab.GetWalls()[i]);
	}
	for (int i = 0; i < lab.BorderElements(); i++) {
		objectManager.AddUndestroyable(lab.GetBorder()[i]);
	}

	LoadStartingObjects();

	int quit = 0;

	timer.NextFrame();

	for (GameObject* go : objectManager.GetAllObjects()) {
		go->Start();
	}

	// Pêtla gry
	while (!quit) {
		// Nowa klatka
		timer.NextFrame();

		if (!input->Update()) {
			return false;
		}

		if (input->PressedThisFrame(SDLK_ESCAPE)) {
			quit = 1;
		}

		//generowanie t³a
		SDL_FillRect(screen, NULL, black);

		for (GameObject* go : objectManager.GetAllObjects()) {
			go->Update();
		}
		lab.Update();

		window.Render();

		objectManager.DisposeDestroyed();
	}

	return true;
}

void Game::Clear() {
	objectManager.Clear();
}

GameObject* Game::CreatePlayer(const Vector& position, bool isControlled) {
	SDL_Surface* screen = window.GetScreen();

	// Obiekt gracza
	GameObject* player = new GameObject(Vector(20, 20), position, objectManager.GetAllObjects());
	player->AddComponent(new SpriteRenderer(*player, screen, bitmaps.playerBmp));
	player->AddComponent(new ConstantMover(*player, PLAYER_SPEED));
	objectManager.AddObject(player);

	// Broñ
	GameObject* basicWeapon = new GameObject(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x,
		objectManager.GetAllObjects()
	);
	basicWeapon->AddComponent(new Firearm(*basicWeapon, basicBullet, WPN_BASIC_RELOAD, FirearmType::Basic));
	basicWeapon->AddComponent(new SpriteRenderer(*basicWeapon, screen, bitmaps.wpnBasicBmp));
	player->AddChild(basicWeapon);
	objectManager.AddObject(basicWeapon);

	// Silna broñ
	GameObject * superWeapon = new GameObject(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x,
		objectManager.GetAllObjects()
	);
	superWeapon->AddComponent(new Firearm(*superWeapon, superBullet, WPN_SUPER_RELOAD, FirearmType::Super));
	superWeapon->AddComponent(new SpriteRenderer(*superWeapon, screen, bitmaps.wpnSuperBmp));
	player->AddChild(superWeapon);
	objectManager.AddObject(superWeapon);

	// Ekwipunek
	player->AddComponent(new PlayerEquipment(*player));
	// Zdrowie
	StatRenderer* healthRenderer = NULL;
	if (isControlled) { // Wyœwietlamy zdrowie tylko gracza kontrolowanego przez tego klienta
		healthRenderer = &healthStats;
	}
	Health* playerHealth = new Health(*player, MAX_HEALTH, healthRenderer);
	player->AddComponent(playerHealth);
	playerHealth->SubscribeDeath(
		[&](Health* deadPlayer) {
			printf("Dead\n");
			objectManager.DestroyObject(&(deadPlayer->GetOwner()));
		}
	);

	if (isControlled) {
		player->AddComponent(new PlayerController(*player));
	}

	return player;
}




GameBitmaps::GameBitmaps() {
	playerBmp = SDL_LoadBMP("resources/player.bmp");
	if (playerBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/player.bmp\n");
		bitmapsOk = false;
	}

	wpnBasicBmp = SDL_LoadBMP("resources/weapon_primary.bmp");
	if (wpnBasicBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/weapon_primary.bmp\n");
		bitmapsOk = false;
	}

	wpnSuperBmp = SDL_LoadBMP("resources/weapon_super.bmp");
	if (wpnSuperBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/weapon_super.bmp\n");
		bitmapsOk = false;
	}

	heartBmp = SDL_LoadBMP("resources/heart.bmp");
	if (heartBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/heart.bmp\n");
		bitmapsOk = false;
	}
}

GameBitmaps::~GameBitmaps() {
	SDL_FreeSurface(playerBmp);
	SDL_FreeSurface(wpnBasicBmp);
	SDL_FreeSurface(wpnSuperBmp);
}

bool GameBitmaps::IsOk() const {
	return bitmapsOk;
}



GameStartInfo::GameStartInfo(Vector* playerPositions, size_t playerCount, int controllableIndex)
	: playerPositions(playerPositions), playerCount(playerCount), controllableIndex(controllableIndex) {

}

GameStartInfo::GameStartInfo(GameStartInfo&& other)
	: playerPositions(other.playerPositions), playerCount(other.playerCount), controllableIndex(other.controllableIndex) {
	other.playerPositions = NULL;
	other.playerCount = 0;
}

GameStartInfo::~GameStartInfo() {
	if (playerPositions != NULL) {
		delete[] playerPositions;
	}
}

Vector GameStartInfo::GetPlayerPosition(int index) const {
	return playerPositions[index];
}

size_t GameStartInfo::GetPlayerCount() const {
	return playerCount;
}

int GameStartInfo::GetControllableIndex() const {
	return controllableIndex;
}