#pragma once
#include "ObjectComponent.h"
#include "InputController.h"
#include "ObjectManager.h"
#include "Bullet.h"

class Firearm :
	public ObjectComponent
{
public:
	Firearm(GameObject& owner, const GameObject& bulletPrefab, float reloadTime, FirearmType type);

	void Update() override;
	// Strzela, jeœli prze³adowano
	bool TryShoot();

	FirearmType GetType() const;

	ObjectComponent* Copy(GameObject& newOwner) override;
protected:
	const GameObject& bulletPrefab;
private:
	float reloadTime;
	float timeSinceLastShot = INFINITY;

	bool isReloaded = true; // czy gotowy do strza³u

	FirearmType type;
};

