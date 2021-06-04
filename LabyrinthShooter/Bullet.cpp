#include "Bullet.h"

Bullet::Bullet(GameObject& owner, float speed, int damage)
	: ObjectComponent(owner), speed(speed), direction(Direction::EAST), damage(damage) {

	gameObject.isStatic = true;

	owner.onCollision =
		[this](GameObject& collider) {
		OnCollision(collider);
	};
}

void Bullet::Update() {
	gameObject.Translate(
		direction * speed * Timer::Main()->GetDeltaTime()
	);
}

void Bullet::OnCollision(GameObject& collider) {
	// Obs�uga trafienia gracza
	Health* playerHealth = collider.FindComponent<Health>();
	if (playerHealth != NULL) {
		playerHealth->Hurt(damage);
	}

	ObjectManager::Main()->DestroyObject(&gameObject);
}

ObjectComponent* Bullet::Copy(GameObject& newOwner) {
	return new Bullet(newOwner, speed, damage);
}

void Bullet::SetDirection(const Vector& direction) {
	this->direction = Vector(direction);
	this->direction.Normalize();
}