#pragma once
#include "GameObject.h"

// Obiekt przechowuj¹cy wszystkie obiekty
class ObjectManager
{
public:
	ObjectManager();

	void AddObject(GameObject* object);
	// Usuwanie obiektu jest zadaniem kogoœ innego
	void AddUndestroyable(GameObject* object);
	void DestroyObject(GameObject* object);

	void DisposeDestroyed();

	const std::list<GameObject*>& GetAllObjects() const;

	~ObjectManager();

	static ObjectManager* Main();
private:
	std::list<GameObject*> allObjects;
	std::list<GameObject*> destroyables;
	std::list<GameObject*> destroyed;

	static ObjectManager* main;
};

