#pragma once
#include "Vector.h"
#include "Timer.h"
#include "IUpdateable.h"
#include <functional>
#include <math.h>

#include <list>
#define BUMPOUT_SPEED 10000.0

class GameObject
{
public:
	bool isEnabled = true;
	bool isStatic = false;
	bool collisionEnabled = true;

	std::function<void(GameObject& collider)> onCollision;
public:
	GameObject(const std::list<GameObject*>& allObjects);
	GameObject(const Vector& size, const std::list<GameObject*>& allObjects);
	GameObject(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects);
	GameObject(const GameObject& other);

	void AddComponent(IUpdateable* component);

	// Raz na klatkê
	void Update();

	const Vector& GetPosition() const;
	const Vector& GetSize() const;
	double GetRotation() const;
	Vector GetMiddle() const;
	bool Collides(const GameObject& other) const;
	bool CollidesWithAny() const;

	void SetPosition(const Vector& newPosition);
	void Translate(const Vector& offset); // przesuniêcie
	void SetSize(const Vector& newSize);
	void Rotate(double angle);

	void AddChild(GameObject* child);


	~GameObject();
protected:
	std::list<IUpdateable*> components;

	bool insideOutCollision = false;
private:
	Vector size;
	Vector position;
	double rotation = 0.0;

	GameObject* parent = NULL;
	std::list<GameObject*> children;

	const std::list<GameObject*>& allObjects;
private:
	void HandleCollisions();
	// Wypycha inny obiekt podczas kolizji
	void BumpOut(GameObject& other);

	bool DoesIntersect(const GameObject& other) const;
	static bool IsInside(const GameObject& go1, const GameObject& go2);
	Rect GetIntersection(const GameObject& other) const;

	/***** Operacje na jednowymiarowych liniach ******/
	static bool IsLineInside(float min1, float max1, float min2, float max2);
	static bool DoLinesIntersect(float min1, float max1, float min2, float max2);
	static Vector LinesIntersection(float min1, float max1, float min2, float max2);
};

