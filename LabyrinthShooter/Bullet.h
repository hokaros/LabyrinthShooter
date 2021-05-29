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

	virtual ObjectComponent* Copy(GameObject& newOwner) override;
protected:
	float speed;
protected:
	virtual void OnCollision(GameObject& collider);
private:
	Vector direction;
};

