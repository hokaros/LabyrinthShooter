#pragma once
#include "ObjectComponent.h"
#include "ObjectManager.h"
#include "Health.h"

// Obiekt biegn�cy przed siebie do momentu kolizji
class Bullet :
	public ObjectComponent
{
public:
	Bullet(GameObject& owner, float speed, int damage);
	void Update() override;

	void SetDirection(const Vector& direction);

	virtual ObjectComponent* Copy(GameObject& newOwner) override;
protected:
	float speed;
	int damage;
protected:
	virtual void OnCollision(GameObject& collider);
private:
	Vector direction;
};

