#include "Firearm.h"

Firearm::Firearm(GameObject& owner, const GameObject& bulletPrefab, float reloadTime)
	: ObjectComponent(owner), bulletPrefab(bulletPrefab), reloadTime(reloadTime) {

	gameObject.collisionEnabled = false;
}

void Firearm::Update() {
	timeSinceLastShot += Timer::Main()->GetDeltaTime();
	if (timeSinceLastShot >= reloadTime) {
		isReloaded = true;
	}

	if (InputController::Main()->IsKeyDown(SHOOT_KEY) && isReloaded) {
		Shoot();
		timeSinceLastShot = 0.0f;
		isReloaded = false;
	}
}

void Firearm::Shoot() {
	// Stworzenie pocisku
	GameObject* bullet = new GameObject(bulletPrefab);

	ObjectManager::Main()->AddObject(bullet);

	// Ustawienie pozycji
	Vector relativePos = Vector(Direction::EAST) * gameObject.GetSize().x;
	bullet->SetPosition(gameObject.LocalToWorld(relativePos));
	// Obrót zgodnie z obrotem broni
	bullet->Rotate(gameObject.GetRotation());

	// Nadanie kierunku lotu
	bullet->FindComponent<Bullet>()->SetDirection(gameObject.LookingDirection());
}

ObjectComponent* Firearm::Copy(GameObject& newOwner) {
	return new Firearm(newOwner, bulletPrefab, reloadTime);
}