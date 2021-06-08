#include "ServerSimulation.h"

ServerSimulation::ServerSimulation(Window& window, Server& server, GameStartInfo&& startInfo)
	: server(server), game(window, std::move(startInfo), true) {
	
	SubscribeToServer();
	SubscribeToGame();
}

void ServerSimulation::Run() {
	game.Run();
}

void ServerSimulation::SubscribeToGame() {
	game.onLabChanged = [this](bool* walls) {
		server.MessageAllClients(Server::CreateMessageLabirynthChange(walls));
	};
}

void ServerSimulation::SubscribeToServer() {
	server.onDirectionChanged = [this](int id, Vector newDir, Vector pos) {
		printf("Direction changed of player %d\n", id);
		if (!game.IsRunning())
			return;

		GameObject* player = game.GetPlayer(id);
		if (player == NULL || player == game.GetControlledPlayer())
			return;

		game.Invoke([player, newDir, pos]() {
			player->SetPosition(pos);
			player->FindComponent<ConstantMover>()->SetDirection(newDir);
			});
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
	server.onShot = [this](int id, double dir, FirearmType wpnType, Vector pos) {
		printf("Player %d shot\n", id);
		if (!game.IsRunning())
			return;

		GameObject* player = game.GetPlayer(id);
		if (player == NULL || player == game.GetControlledPlayer())
			return;

		game.Invoke([player, dir, wpnType, pos]() {
			player->SetRotation(dir);
			player->SetPosition(pos);
			PlayerEquipment* eq = player->FindComponent<PlayerEquipment>();
			eq->EquipWeapon(wpnType);
			eq->GetCurrentWeapon()->TryShoot();
			});
	};
	server.onWeaponChanged = [this](int id, FirearmType newType) {
		printf("Player %d changed weapon\n", id);
		if (!game.IsRunning())
			return;

		GameObject* player = game.GetPlayer(id);
		if (player == NULL || player == game.GetControlledPlayer())
			return;

		game.Invoke([player, newType]() { player->FindComponent<PlayerEquipment>()->EquipWeapon(newType); });
	};
}