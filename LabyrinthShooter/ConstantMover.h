#pragma once
#include "ObjectComponent.h"

#define DIRECTION_TOLERANCE 0.001

class ConstantMover :
	public ObjectComponent
{
public:
	ConstantMover(GameObject& owner, float movementSpeed);

	void Update() override;

	void SetDirection(const Vector& newDir);
	// Czy obiekt porusza siê w okreœlon¹ stronê
	bool IsSameDirection(const Vector& otherDir) const;

	ObjectComponent* Copy(GameObject& newOwner) override;

protected:
	// Prêdkoœæ [piksele na sekundê]
	float movementSpeed;
	Vector moveDir;
};