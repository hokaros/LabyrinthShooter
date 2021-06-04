#include "Health.h"

Health::Health(GameObject& owner, int maxHealth)
	: ObjectComponent(owner), maxHealth(maxHealth), currHealth(maxHealth) {

}

ObjectComponent* Health::Copy(GameObject& newOwner)
{
	Health* health = new Health(newOwner, maxHealth);
	// Dodanie funkcji obs³ugi œmierci
	for (function<void(Health*)> deathHandler : onDeath) {
		health->SubscribeDeath(deathHandler);
	}
	return health;
}

void Health::Hurt(int hp) {
	currHealth -= hp;
	printf("Ouch, curr = %d\n", currHealth);

	if (currHealth <= 0) {
		OnDeath();
	}
}

void Health::OnDeath() {
	for (function<void(Health*)> handler : onDeath) {
		if (handler) {
			handler(this);
		}
	}
}

void Health::SubscribeDeath(function<void(Health*)> handler) {
	onDeath.push_back(handler);
}
