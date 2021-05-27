#include "Bullet.h"

Bullet::Bullet(GameObject& owner, float speed)
	: ObjectComponent(owner), speed(speed) {

	owner.onCollision =
		[this](GameObject& collider) {
		OnCollision(collider);
	};
}

void Bullet::Update() {
	gameObject.Translate(
		Vector(Direction::EAST) * speed * Timer::Main()->GetDeltaTime()
	);
}

void Bullet::OnCollision(GameObject& collider) {
	// Sprawdzenie, czy gracz
}

ObjectComponent* Bullet::Copy(GameObject& newOwner) {
	return new Bullet(newOwner, speed);
}