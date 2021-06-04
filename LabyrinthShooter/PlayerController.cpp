#include "PlayerController.h"
#include "Health.h"

PlayerController::PlayerController(GameObject& player, float movementSpeed)
	: ObjectComponent(player), movementSpeed(movementSpeed) {

}

void PlayerController::Start() {
	// Za�adowanie cache'a
	input = InputController::Main();
	equipment = gameObject.FindComponent<PlayerEquipment>();
}

void PlayerController::Update() {
	ProcessMovement();
	ProcessAim();

	// Zmiana broni
	if (input->PressedThisFrame(WPN_SWITCH_KEY)) {
		equipment->SwitchWeapon();
	}

	// Strzelanie
	if (input->IsKeyDown(SHOOT_KEY) && equipment->GetCurrentWeapon() != NULL) {
		equipment->GetCurrentWeapon()->TryShoot();
	}

	// XD
	if (input->PressedThisFrame(SDLK_s)) {
		gameObject.FindComponent<Health>()->Hurt(1);
	}
}

ObjectComponent* PlayerController::Copy(GameObject& newOwner) {
	return new PlayerController(newOwner, movementSpeed);
}

void PlayerController::ProcessMovement() {
	// Odczytanie wci�ni�tych klawiszy ruchu
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
	Vector dPos = moveDir * movementSpeed * Timer::Main()->GetDeltaTime();
	gameObject.SetPosition(gameObject.GetPosition() + dPos);
}

void PlayerController::ProcessAim() {
	Vector mousePos = input->GetMousePosition();

	gameObject.LookAt(mousePos);
}