#include "ServerSimulation.h"

ServerSimulation::ServerSimulation(Window* window, Server& server, GameStartInfo&& startInfo)
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
	game.onPlayerHurt = [this](int id, int dmg) {
		server.MessageAllClients(Server::CreateMessagePlayerHurt(id, dmg));
	};
}

void ServerSimulation::SubscribeToServer() {
	server.onDirectionChanged = [this](int playerId, Vector newDir, Vector pos) {
		OnDirectionChanged(playerId, newDir, pos);
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
	server.onShot = [this](int playerId, double dir, FirearmType wpnType, Vector pos) {
		OnShot(playerId, dir, wpnType, pos);
	};
	server.onWeaponChanged = [this](int playerId, FirearmType newType) {
		OnWeaponChanged(playerId, newType);
	};
}

void ServerSimulation::OnDirectionChanged(int playerId, Vector newDir, Vector pos)
{
	printf("Direction changed of player %d\n", playerId);
	if (!game.IsRunning())
		return;

	GameObject* player = game.GetPlayer(playerId);
	if (player == NULL || player == game.GetControlledPlayer())
		return;

	game.InvokeOnNextFrame([player, newDir, pos]() {
		player->SetPosition(pos);
		player->FindComponent<ConstantMover>()->SetDirection(newDir);
		});
}

void ServerSimulation::OnShot(int playerId, double dir, FirearmType wpnType, Vector pos)
{
	printf("Player %d shot\n", playerId);
	if (!game.IsRunning())
		return;

	GameObject* player = game.GetPlayer(playerId);
	if (player == NULL || player == game.GetControlledPlayer())
		return;

	game.InvokeOnNextFrame([player, dir, wpnType, pos]() {
		player->SetRotation(dir);
		player->SetPosition(pos);
		PlayerEquipment* eq = player->FindComponent<PlayerEquipment>();
		eq->EquipWeapon(wpnType);
		eq->GetCurrentWeapon()->TryShoot();
		});
}


void ServerSimulation::OnWeaponChanged(int playerId, FirearmType newType)
{
	printf("Player %d changed weapon\n", playerId);
	if (!game.IsRunning())
		return;

	GameObject* player = game.GetPlayer(playerId);
	if (player == NULL || player == game.GetControlledPlayer())
		return;

	game.InvokeOnNextFrame([player, newType]() { player->FindComponent<PlayerEquipment>()->EquipWeapon(newType); });
}