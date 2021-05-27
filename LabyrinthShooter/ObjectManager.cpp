#include "ObjectManager.h"

ObjectManager* ObjectManager::main{ NULL };

ObjectManager* ObjectManager::Main() {
	return main;
}

ObjectManager::ObjectManager() {
	if (main == NULL) {
		main = this;
	}
}

ObjectManager::~ObjectManager() {
	for (GameObject* go : destroyables) {
		delete go;
	}

	if (main == this) {
		main = NULL;
	}
}

void ObjectManager::AddObject(GameObject* gameObject) {
	allObjects.push_back(gameObject);

	destroyables.push_back(gameObject);
}

void ObjectManager::AddUndestroyable(GameObject* gameObject) {
	allObjects.push_back(gameObject);
}

const std::list<GameObject*>& ObjectManager::GetAllObjects() const {
	return allObjects;
}