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

	const std::list<GameObject*>& GetAllObjects() const;

	~ObjectManager();

	static ObjectManager* Main();
private:
	std::list<GameObject*> allObjects;
	std::list<GameObject*> destroyables;

	static ObjectManager* main;
};

