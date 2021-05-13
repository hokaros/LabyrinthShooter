#include "GameObject.h"

GameObject::GameObject(const std::list<GameObject*>& allObjects)
	: size(1, 1), allObjects(allObjects) {

}

GameObject::GameObject(const Vector& size, const std::list<GameObject*>& allObjects)
	: size(size), allObjects(allObjects) {

}

GameObject::GameObject(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects)
	: position(position), size(size), allObjects(allObjects) {

}

GameObject::~GameObject() {
	for (IUpdateable* component : components) {
		delete component;
	}
}

void GameObject::AddComponent(IUpdateable* component) {
	components.push_back(component);
}

void GameObject::Update() {
	for (IUpdateable* component : components) {
		component->Update();
	}
}

const Vector& GameObject::GetSize() const {
	return size;
}

const Vector& GameObject::GetPosition() const {
	return position;
}

void GameObject::SetPosition(const Vector& newPosition) {
	Vector prevPosition = position;

	position = Vector(newPosition);

	if (CollidesWithAny()) {
		position = prevPosition;
	}
}

bool GameObject::Collides(const GameObject& other) const {
	if (!collisionEnabled || !other.collisionEnabled)
		return false;

	// Kolizje prostok¹tów
	float yMin1 = position.y;
	float yMax1 = yMin1 + size.y;
	float yMin2 = other.position.y;
	float yMax2 = yMin2 + other.size.y;
	
	bool yIntersect = DoLinesIntersect(yMin1, yMax1, yMin2, yMax2);
	bool yInside = IsLineInside(yMin1, yMax1, yMin2, yMax2);

	float xMin1 = position.x;
	float xMax1 = xMin1 + size.x;
	float xMin2 = other.position.x;
	float xMax2 = xMin2 + other.size.x;

	bool xIntersect = DoLinesIntersect(xMin1, xMax1, xMin2, xMax2);
	bool xInside = IsLineInside(xMin1, xMax1, xMin2, xMax2);

	// Rozpatrzenie kolizji odwrotnych (mog¹ siê znosiæ)
	bool inverse = false;
	if (insideOutCollision)
		inverse = !inverse;
	if (other.insideOutCollision)
		inverse = !inverse;

	if (inverse) {
		if (xInside && yInside)
			return false;  // wewn¹trz
		if (!xIntersect || !yIntersect)
			return true;  // na zewn¹trz
	}

	return xIntersect && yIntersect;
}

bool GameObject::CollidesWithAny() const {
	for (GameObject* go : allObjects) {
		if (go != this && Collides(*go))
			return true;
	}

	return false;
}

bool GameObject::IsLineInside(float min1, float max1, float min2, float max2) {
	return (min1 >= min2 && max1 <= max2)
		|| (min2 >= min1 && max2 <= max1);
}
bool GameObject::DoLinesIntersect(float min1, float max1, float min2, float max2) {
	return (min1 >= min2 && min1 <= max2)
		|| (max1 >= min2 && max1 <= max2);
}