#include "Regenerable.h"

Regenerable::Regenerable(GameObject& owner, double regenerationTime)
	: ObjectComponent(owner), regenerationTime(regenerationTime) {
	gameObject.onDestroyed = [this] () {OnDestroyed(); };
}

void Regenerable::Update() {

}

void Regenerable::OnDestroyed() {
	Timer::Main()->Invoke([this]() {
		gameObject.SetDestroyed(false);
		}, regenerationTime);
}

ObjectComponent* Regenerable::Copy(GameObject& newOwner) {
	return new Regenerable(newOwner, regenerationTime);
}