#pragma once
#include "GameObject.h"

// Obiekt przechowuj�cy wszystkie obiekty
class ObjectManager
{
public:
	ObjectManager();

	// Dodaje obiekt do zarz�dzanych wraz z ca�ym drzewem dzieci
	void AddObject(GameObject* object);
	// Usuwanie obiektu jest zadaniem kogo� innego
	void AddUndestroyable(GameObject* object);
	void DestroyObject(GameObject* object, bool detach=true);

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

