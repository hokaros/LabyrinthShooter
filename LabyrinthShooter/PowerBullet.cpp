#include "PowerBullet.h"

PowerBullet::PowerBullet(GameObject& owner, float speed)
	: Bullet(owner, speed), health(POWER_BULLET_HEALTH) {

}

ObjectComponent* PowerBullet::Copy(GameObject& newOwner) {
	return new PowerBullet(newOwner, speed);
}

void PowerBullet::OnCollision(GameObject& collider) {
	// TODO: sprawiæ, aby nie odbija³ siê od obiektów
	if (collider.isDestroyable) {
		collider.SetDestroyed(true);

		DecreaseHealth();
	}
	else {
		Bullet::OnCollision(collider);
	}
}

void PowerBullet::DecreaseHealth() {
	health--;
	if (health <= 0) {
		ObjectManager::Main()->DestroyObject(&gameObject);
	}
}