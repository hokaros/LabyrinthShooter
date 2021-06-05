#include "Game.h"

// Input
// Nale¿y tu dodaæ wszystkie klawisze, które chce siê odczytywaæ
SDL_KeyCode steeringKeys[] = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_a, SDLK_w, SDLK_d, SDLK_s,
	SHOOT_KEY,
	WPN_SWITCH_KEY
};

Game::Game(Window& window)
	: window(window),
	input(steeringKeys, sizeof(steeringKeys) / sizeof(SDL_KeyCode)),
	basicBullet(Vector(4, 4), objectManager.GetAllObjects()), // uwa¿aæ przy zmienianiu objectManagera
	superBullet(Vector(10, 10), objectManager.GetAllObjects()) {

	SDL_Surface* screen = window.GetScreen();

	int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int yellow = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);

	basicBullet.AddComponent(new Bullet(basicBullet, BULLET_BASIC_SPEED, BULLET_BASIC_DAMAGE));
	basicBullet.AddComponent(new RectangleRenderer(basicBullet, screen, yellow, yellow));

	superBullet.AddComponent(new PowerBullet(superBullet, BULLET_SUPER_SPEED, BULLET_SUPER_DAMAGE));
	superBullet.AddComponent(new RectangleRenderer(superBullet, screen, red, red));
}

void Game::LoadStartingObjects() {
	SDL_Surface* screen = window.GetScreen();

	// TODO: pobieranie pozycji wejœciowej gracza
	GameObject* player = new GameObject(Vector(20, 20), Vector(30, 250), objectManager.GetAllObjects());
	player->AddComponent(new SpriteRenderer(*player, screen, bitmaps.playerBmp));
	player->AddComponent(new PlayerController(*player, 300.0f));
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
	GameObject* superWeapon = new GameObject(
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
	Health* playerHealth = new Health(*player, MAX_HEALTH);
	player->AddComponent(playerHealth);
	playerHealth->SubscribeDeath(
		[&](Health* deadPlayer) {
			printf("Dead\n");
			objectManager.DestroyObject(&(deadPlayer->GetOwner()));
		}
	);

	GameObject* player2 = new GameObject(*player);
	player2->SetPosition(Vector(130, 250));
	objectManager.AddObject(player2);
}

void Game::Run() {
	SDL_Surface* screen = window.GetScreen();
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	Vector mapStart(10, 10);
	LabyrinthSolidifier lab(mapStart, 10, 100, 5, 5, objectManager.GetAllObjects(), LAB_TIME);
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

		if (!input.Update()) {
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
}

void Game::Clear() {
	objectManager.Clear();
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
}

GameBitmaps::~GameBitmaps() {
	SDL_FreeSurface(playerBmp);
	SDL_FreeSurface(wpnBasicBmp);
	SDL_FreeSurface(wpnSuperBmp);
}

bool GameBitmaps::IsOk() const {
	return bitmapsOk;
}