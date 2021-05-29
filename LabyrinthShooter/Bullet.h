#pragma once
#include "ObjectComponent.h"
#include "ObjectManager.h"

// Obiekt biegn¹cy przed siebie do momentu kolizji
class Bullet :
	public ObjectComponent
{
public:
	Bullet(GameObject& owner, float speed);
	void Update() override;

	void SetDirection(const Vector& direction);

	ObjectComponent* Copy(GameObject& newOwner) override;
protected:
	virtual void OnCollision(GameObject& collider);
private:
	float speed;
	Vector direction;
};

