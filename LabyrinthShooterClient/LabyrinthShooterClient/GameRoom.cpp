#include "GameRoom.h"

GameRoom::GameRoom(Window& window, Client* client)
	: window(window), playerCount(1), client(client) {
	SubscribeToClient();
}

void GameRoom::Enter() {
	SDL_Surface* screen = window.GetScreen();
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	InputController* input = InputController::Main();

	bool quit = false;
	bool shouldStart = false;

	while (!quit) {
		if (!input->Update()) {
			quit = 1;
		}

		// Generowanie t³a
		SDL_FillRect(screen, NULL, black);

		// Wyœwietlenie poczekalni
		DrawWaitingRoom();

		{
			// Odczytanie zewnêtrznego sygna³u rozpoczêcia
			std::lock_guard<std::mutex> lock(mutex);
			shouldStart = startGame;
		}

		if (shouldStart) {
			// Wejœcie do gry
			RunGame();
			std::lock_guard<std::mutex> lock(mutex);
			startGame = false;
		}

		if (input->PressedThisFrame(SDLK_RETURN)) {
			// Testowe wejœcie do gry
			SetNextGame(new Game(window, 
				GameStartInfo(new Vector[1]{ Vector(50, 200) }, 1, 0))
			);
			RunGame();
		}
		if (input->PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		window.Render();
	}
}

void GameRoom::DrawWaitingRoom() {
	char buffer[16];
	// Informacja o liczbie graczy w poczekalni
	sprintf_s(buffer, "%d/%d", GetPlayerCount(), NEEDED_PLAYERS);
	window.DrawString(5, 5, buffer, FONTSIZE_MEDIUM);

	window.DrawString(window.GetWidth()/2 - 15*FONTSIZE_MEDIUM, window.GetHeight()/2 - FONTSIZE_MEDIUM, "Oczekiwanie na graczy...", FONTSIZE_MEDIUM);
}

void GameRoom::StartGame(int selfId, Vector* playerPositions, size_t playerCount) {
	this->playerCount = playerCount;

	Vector* positionsCpy = new Vector[playerCount];
	std::memcpy(positionsCpy, playerPositions, playerCount * sizeof(Vector));
	GameStartInfo gameInfo(positionsCpy, playerCount, selfId);

	std::lock_guard<std::mutex> lock(mutex);
	nextGame = new Game(window, std::move(gameInfo));
	startGame = true;
}

void GameRoom::RunGame() {
	if (nextGame == NULL)
		return;

	{
		std::lock_guard<std::mutex> lock(mutex);
		// Ustawienie przygotowanej gry jako aktualna
		if (game != NULL) {
			delete game;
			game = NULL;
		}
		game = nextGame;
		nextGame = NULL;
	}

	SubscribeToGame();
	game->Run();

	delete game;
	game = NULL;
}

int GameRoom::GetPlayerCount() {
	std::lock_guard<std::mutex> lock(mutex);

	return playerCount;
}

void GameRoom::SetPlayerCount(int count) {
	std::lock_guard<std::mutex> lock(mutex);

	playerCount = count;
}

void GameRoom::SetShouldStart(bool shouldStart) {
	std::lock_guard<std::mutex> lock(mutex);

	startGame = shouldStart;
}

bool GameRoom::ShouldStart() {
	std::lock_guard<std::mutex> lock(mutex);
	return startGame;
}

void GameRoom::SetNextGame(Game* newGame) {
	std::lock_guard<std::mutex> lock(mutex);
	nextGame = newGame;
}

void GameRoom::OnPlayerJoined() {
	SetPlayerCount(GetPlayerCount() + 1);
}

void GameRoom::OnPlayerLeft() {
	SetPlayerCount(GetPlayerCount() - 1);
}

void GameRoom::OnGameStarted(int selfId, float positions[PLAYERS_NUM][2], bool* walls) {
	Vector playerPositions[PLAYERS_NUM];
	for (int i = 0; i < PLAYERS_NUM; i++) {
		playerPositions[i] = Vector(positions[i][0], positions[i][1]);
	}
	StartGame(selfId, playerPositions, PLAYERS_NUM);
}

void GameRoom::SubscribeToClient() {
	client->onPlayerJoined = [this]() {OnPlayerJoined(); };
	client->onPlayerLeft = [this]() {OnPlayerLeft(); };
	client->onGameStarted = [this](int selfId, float positions[PLAYERS_NUM][2], bool* walls) {OnGameStarted(selfId, positions, walls); };

	client->onDirectionChanged = [this](int id, Vector newDir) {
		printf("Direction changed of player %d\n", id);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL || player == game->GetControlledPlayer())
			return;

		game->Invoke([player, newDir]() { player->FindComponent<ConstantMover>()->SetDirection(newDir); });
	};/*
	client->onAimChanged = [this](int id, double rotation) {
		printf("Aiming changed of player %d\n", id);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL || player == game->GetControlledPlayer())
			return;

		game->Invoke([player, rotation]() { player->SetRotation(rotation); });
	};*/
	client->onShot = [this](int id) {
		printf("Player %d shot\n", id);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL || player == game->GetControlledPlayer())
			return;

		game->Invoke([player]() { player->FindComponent<PlayerEquipment>()->GetCurrentWeapon()->TryShoot(); });
	};
	client->onWeaponChanged = [this](int id, FirearmType newType) {
		printf("Player %d changed weapon\n", id);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL || player == game->GetControlledPlayer())
			return;

		game->Invoke([player, newType]() { player->FindComponent<PlayerEquipment>()->EquipWeapon(newType); });
	};
}

void GameRoom::SubscribeToGame() {
	game->onControlledDirectionChanged = [this](const Vector& newDir) {
		if (client == NULL)
			return;

		client->Send(Client::CreateMessageNewDirection(newDir));
	};/*
	game->onControlledAimChanged = [this](double newRot) {
		if (client == NULL)
			return;

		client->Send(Client::CreateMessageChangeOfAimingDirection(newRot));
	};*/
	game->onControlledShot = [this]() {
		if (client == NULL)
			return;

		client->Send(Client::CreateMessagePlayerShot());
	};
	game->onControlledWeaponChanged = [this](FirearmType newType) {
		if (client == NULL)
			return;

		client->Send(Client::CreateMessageWeaponChange(newType));
	};
}


RoomFinder::RoomFinder(Window& window)
	: window(window) {

}

RoomFinder::~RoomFinder() {
	if (client != NULL) {
		delete client;
	}
}

void RoomFinder::EnterSearch() {
	SDL_Surface* screen = window.GetScreen();
	int white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	SDL_Rect textArea;
	textArea.x = 50;
	textArea.y = window.GetHeight() / 2;
	textArea.w = window.GetWidth() - 2 * 50;
	textArea.h = 50;
	TextBox textBox(textArea, white, black, 20);


	InputController* input = InputController::Main();

	bool quit = false;

	while (!quit) {

		if (!input->Update()) {
			quit = 1;
		}

		if (input->PressedThisFrame(SDLK_RETURN)) {
			// Testowa próba po³¹czenia z serwerem
			TryConnect(textBox.GetContent());
		}
		else if (input->PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		// Generowanie t³a
		SDL_FillRect(screen, NULL, black);

		textBox.Update();

		// Wyœwietlenie wyszukiwarki
		Draw(textBox);

		window.Render();


		if (ShouldEnter()) {
			RealEnterGameRoom();
		}
	}
}

void RoomFinder::EnterGameRoom() {
	std::lock_guard<std::mutex> lock(mutex);

	shouldEnter = true;
}

void RoomFinder::RealEnterGameRoom() {
	GameRoom gameRoom(window, client);
	SetCurrentRoom(&gameRoom);

	gameRoom.Enter();

	// Po wyjœciu z pokoju
	client->Disconnect();
	SetCurrentRoom(NULL);
}

GameRoom* RoomFinder::GetCurrentRoom() {
	std::lock_guard<std::mutex> lock(mutex);

	return currentRoom;
}

void RoomFinder::SetCurrentRoom(GameRoom* newRoom) {
	std::lock_guard<std::mutex> lock(mutex);

	currentRoom = newRoom;
}

void RoomFinder::TryConnect(std::string ip) {
	if (client != NULL)
		delete client;

	client = new Client(ip.c_str(), DEFAULT_PORT);
	client->onJoinAccepted = [this]() {EnterGameRoom(); };
	client->Connect();
	client->GameProtocol();
}

bool RoomFinder::ShouldEnter() {
	std::lock_guard<std::mutex> lock(mutex);

	bool should = shouldEnter;
	shouldEnter = false;
	return should;
}

void RoomFinder::Draw(TextBox& textBox) {
	window.DrawString(window.GetWidth() / 2 - FONTSIZE_HEADER * 9, 50, "Podaj ip serwera:", FONTSIZE_HEADER);

	textBox.Draw();
}