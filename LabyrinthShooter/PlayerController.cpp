#include "PlayerController.h"

PlayerController::PlayerController(GameObject& player, float movementSpeed)
	: ObjectComponent(player), movementSpeed(movementSpeed) {

}

void PlayerController::Update() {
	InputController* input = InputController::Main();

	// Odczytanie wciœniêtych klawiszy
	Vector moveDir;
	if (input->IsKeyDown(SDLK_UP) || input->IsKeyDown(SDLK_w)) {
		moveDir.y -= 1;
	}
	if (input->IsKeyDown(SDLK_DOWN) || input->IsKeyDown(SDLK_s)) {
		moveDir.y += 1;
	}
	if (input->IsKeyDown(SDLK_LEFT) || input->IsKeyDown(SDLK_a)) {
		moveDir.x -= 1;
	}
	if (input->IsKeyDown(SDLK_RIGHT) || input->IsKeyDown(SDLK_d)) {
		moveDir.x += 1;
	}

	// Ruch
	moveDir.Normalize();
	gameObject.position += moveDir * movementSpeed * Timer::Main()->GetDeltaTime();
}