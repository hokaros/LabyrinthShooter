#pragma once
#include "Vector.h"
#include "Timer.h"
#include "IUpdateable.h"

#include <list>
#define BUMPOUT_SPEED 10000.0

class GameObject
{
public:
	bool isStatic = false;
public:
	GameObject(const std::list<GameObject*>& allObjects);
	GameObject(const Vector& size, const std::list<GameObject*>& allObjects);
	GameObject(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects);

	void AddComponent(IUpdateable* component);

	// Raz na klatkê
	void Update();

	const Vector& GetPosition() const;
	const Vector& GetSize() const;
	Vector GetMiddle() const;
	bool Collides(const GameObject& other) const;
	bool CollidesWithAny() const;

	void SetPosition(const Vector& newPosition);
	void SetSize(const Vector& newSize);


	~GameObject();
protected:
	std::list<IUpdateable*> components;

	bool collisionEnabled = true;
	bool insideOutCollision = false;
private:
	Vector size;
	Vector position;

	const std::list<GameObject*>& allObjects;
private:
	// Wypycha inne obiekty, jeœli koliduj¹
	void BumpOut();
	void BumpOut(GameObject& other);

	bool DoesIntersect(const GameObject& other) const;
	static bool IsInside(const GameObject& go1, const GameObject& go2);
	Rect GetIntersection(const GameObject& other) const;

	/***** Operacje na jednowymiarowych liniach ******/
	static bool IsLineInside(float min1, float max1, float min2, float max2);
	static bool DoLinesIntersect(float min1, float max1, float min2, float max2);
	static Vector LinesIntersection(float min1, float max1, float min2, float max2);
};

