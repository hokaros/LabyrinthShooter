#include "PowerBullet.h"

PowerBullet::PowerBullet(GameObject& owner, float speed)
	: Bullet(owner, speed), health(POWER_BULLET_HEALTH) {
	gameObject.isStatic = true;
}

ObjectComponent* PowerBullet::Copy(GameObject& newOwner) {
	return new PowerBullet(newOwner, speed);
}

void PowerBullet::OnCollision(GameObject& collider) {
	if (collider.isDestroyable) {
		collider.SetDestroyed(true);

		DecreaseHealth(); // TODO: zamieniæ na zmniejszanie si³y wraz z przebytym dystansem
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