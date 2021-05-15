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

	BumpOut();
}

const Vector& GameObject::GetSize() const {
	return size;
}

const Vector& GameObject::GetPosition() const {
	return position;
}

Vector GameObject::GetMiddle() const {
	return position + size / 2;
}

void GameObject::SetPosition(const Vector& newPosition) {
	position = newPosition;
}

void GameObject::SetSize(const Vector& newSize) {
	size.x = newSize.x;
	size.y = newSize.y;
}

bool GameObject::Collides(const GameObject& other) const {
	if (!collisionEnabled || !other.collisionEnabled)
		return false;

	// Rozpatrzenie kolizji odwrotnych (mog¹ siê znosiæ)
	bool inverse = false;
	if (insideOutCollision)
		inverse = !inverse;
	if (other.insideOutCollision)
		inverse = !inverse;

	bool intersect = DoesIntersect(other);
	if (inverse) {
		if (IsInside(*this, other))
			return false;  // wewn¹trz
		if (intersect)
			return true;  // na zewn¹trz
	}

	return intersect;
}

bool GameObject::CollidesWithAny() const {
	for (GameObject* go : allObjects) {
		if (go != this && Collides(*go))
			return true;
	}

	return false;
}

bool GameObject::DoesIntersect(const GameObject& other) const {
	float yMin1 = position.y;
	float yMax1 = yMin1 + size.y;
	float yMin2 = other.position.y;
	float yMax2 = yMin2 + other.size.y;

	if (!DoLinesIntersect(yMin1, yMax1, yMin2, yMax2))
		return false;

	float xMin1 = position.x;
	float xMax1 = xMin1 + size.x;
	float xMin2 = other.position.x;
	float xMax2 = xMin2 + other.size.x;

	if (!DoLinesIntersect(xMin1, xMax1, xMin2, xMax2))
		return false;

	return true;
}

bool GameObject::IsInside(const GameObject& go1, const GameObject& go2) {
	float yMin1 = go1.position.y;
	float yMax1 = yMin1 + go1.size.y;
	float yMin2 = go2.position.y;
	float yMax2 = yMin2 + go2.size.y;

	if (!IsLineInside(yMin1, yMax1, yMin2, yMax2))
		return false;


	float xMin1 = go1.position.x;
	float xMax1 = xMin1 + go1.size.x;
	float xMin2 = go2.position.x;
	float xMax2 = xMin2 + go2.size.x;

	if (!IsLineInside(xMin1, xMax1, xMin2, xMax2))
		return false;

	return true;
}

Rect GameObject::GetIntersection(const GameObject& other) const {
	float yMin1 = position.y;
	float yMax1 = yMin1 + size.y;
	float yMin2 = other.position.y;
	float yMax2 = yMin2 + other.size.y;

	Vector yIntersection = LinesIntersection(yMin1, yMax1, yMin2, yMax2);

	float xMin1 = position.x;
	float xMax1 = xMin1 + size.x;
	float xMin2 = other.position.x;
	float xMax2 = xMin2 + other.size.x;

	Vector xIntersection = LinesIntersection(xMin1, xMax1, xMin2, xMax2);

	Vector size(
		xIntersection.y - xIntersection.x,
		yIntersection.y - yIntersection.x
	);
	return Rect(
		Vector(xIntersection.x, yIntersection.x),
		size
	);
}

bool GameObject::IsLineInside(float min1, float max1, float min2, float max2) {
	return (min1 >= min2 && max1 <= max2)
		|| (min2 >= min1 && max2 <= max1);
}
bool GameObject::DoLinesIntersect(float min1, float max1, float min2, float max2) {
	return (min1 >= min2 && min1 <= max2)
		|| (max1 >= min2 && max1 <= max2)
		|| (min2 >= min1 && min2 <= max1)
		|| (max2 >= min1 && max2 <= max1);
}

Vector GameObject::LinesIntersection(float min1, float max1, float min2, float max2) {
	if (!DoLinesIntersect(min1, max1, min2, max2))
		return Vector(0, 0);

	float start = (min1 < min2) ? min2 : min1;
	float end = (max1 < max2) ? max1 : max2;
	return Vector(start, end);
}

void GameObject::BumpOut() {
	for (GameObject* go : allObjects) {
		if (go == this || go->isStatic)
			continue;

		if (Collides(*go)) {
			BumpOut(*go);
		}
	}
}

void GameObject::BumpOut(GameObject& other) {
	// Tylko jeœli koliduje z other
	if (other.position.y < position.y)
		int x = 1;
	Rect intersection = GetIntersection(other);

	Vector outVector = other.GetMiddle() - intersection.GetMiddle();
	if (outVector.Length() == 0) {
		outVector = Vector(0, -FLT_EPSILON);
	}

	float speed = BUMPOUT_SPEED / outVector.Length(); // im bli¿ej, tym bardziej odpycha
	outVector.Normalize();
	Vector dPos = outVector * speed * Timer::Main()->GetDeltaTime();
	other.position += dPos;
}