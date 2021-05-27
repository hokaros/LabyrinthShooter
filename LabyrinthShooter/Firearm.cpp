#include "Firearm.h"

Firearm::Firearm(GameObject& owner, const GameObject& bulletPrefab, float reloadTime)
	: ObjectComponent(owner), bulletPrefab(bulletPrefab), reloadTime(reloadTime) {

}

void Firearm::Update() {
	if (InputController::Main()->IsKeyDown(SHOOT_KEY) && timeSinceLastShot >= reloadTime) {
		Shoot();
		timeSinceLastShot = 0.0f;
	}

	timeSinceLastShot += Timer::Main()->GetDeltaTime();
}

void Firearm::Shoot() {
	// Stworzenie pocisku
	GameObject* bullet = new GameObject(bulletPrefab);

	ObjectManager::Main()->AddObject(bullet);

	// Ustawienie pozycji
	bullet->SetPosition(gameObject.GetPosition() + Vector(Direction::EAST) * gameObject.GetSize().x);
	// TODO: obrócenie pocisku zgodnie z obrotem broni
}

ObjectComponent* Firearm::Copy(GameObject& newOwner) {
	return new Firearm(newOwner, bulletPrefab, reloadTime);
}