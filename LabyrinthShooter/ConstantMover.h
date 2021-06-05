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
	// Czy obiekt porusza si� w okre�lon� stron�
	bool IsSameDirection(const Vector& otherDir) const;

	ObjectComponent* Copy(GameObject& newOwner) override;

protected:
	// Pr�dko�� [piksele na sekund�]
	float movementSpeed;
	Vector moveDir;
};