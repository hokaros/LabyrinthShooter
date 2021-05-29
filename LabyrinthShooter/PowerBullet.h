#pragma once
#include "Bullet.h"
// Liczba �cian, kt�re mo�e zniszczy� jeden pocisk
#define POWER_BULLET_HEALTH 2

class PowerBullet :
	public Bullet
{
public:
	PowerBullet(GameObject& owner, float speed);

	virtual ObjectComponent* Copy(GameObject& newOwner) override;
protected:
	virtual void OnCollision(GameObject& collider) override;
private:
	int health;
private:
	void DecreaseHealth();
};

