#pragma once
#include "Vector.h"
#include "Timer.h"
#include "IUpdateable.h"

#include <list>

class GameObject
{
public:
	Vector position;
public:
	GameObject();
	GameObject(const Vector& size);
	GameObject(const Vector& size, const Vector& position);

	void AddComponent(IUpdateable* component);

	// Raz na klatkê
	void Update();

	Vector GetSize() const;

	~GameObject();
protected:
	std::list<IUpdateable*> components;
private:
	Vector size;
};

