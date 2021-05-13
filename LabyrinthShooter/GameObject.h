#pragma once
#include "Vector.h"
#include "Timer.h"
#include "IUpdateable.h"

#include <list>

class GameObject
{
public:
	GameObject(const std::list<GameObject*>& allObjects);
	GameObject(const Vector& size, const std::list<GameObject*>& allObjects);
	GameObject(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects);

	void AddComponent(IUpdateable* component);

	// Raz na klatkê
	void Update();

	const Vector& GetPosition() const;
	const Vector& GetSize() const;
	bool Collides(const GameObject& other) const;
	bool CollidesWithAny() const;

	void SetPosition(const Vector& newPosition);

	~GameObject();
protected:
	std::list<IUpdateable*> components;
	// Dodatkowe parametry kolizji
	bool collisionEnabled = true;
	bool insideOutCollision = false;
private:
	Vector size;
	Vector position;

	const std::list<GameObject*>& allObjects;

	static bool IsLineInside(float min1, float max1, float min2, float max2);
	static bool DoLinesIntersect(float min1, float max1, float min2, float max2);
};

