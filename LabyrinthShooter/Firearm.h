#pragma once
#include "ObjectComponent.h"
#include "InputController.h"
#include "ObjectManager.h"

#define SHOOT_KEY SDLK_SPACE

class Firearm :
	public ObjectComponent
{
public:
	Firearm(GameObject& owner, const GameObject& bulletPrefab, float reloadTime);

	void Update() override;

	ObjectComponent* Copy(GameObject& newOwner) override;
protected:
	const GameObject& bulletPrefab;
private:
	float reloadTime;
	float timeSinceLastShot = INFINITY;
private:
	void Shoot();
};

