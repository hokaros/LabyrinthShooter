#pragma once
#include "GameObject.h"

// Kolizje wyst�puj� na zewn�trz, nie w �rodku obiektu
class Cage :
	public GameObject
{
public:
	Cage(const Vector& size, const std::list<GameObject*>& allObjects);
	Cage(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects);
};

