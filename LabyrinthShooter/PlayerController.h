#pragma once
#include "ObjectComponent.h"
#include "InputController.h"

class PlayerController :
	public ObjectComponent
{
public:
	PlayerController(GameObject& player, float movementSpeed);
	void Update() override;
protected:
	// Pr�dko�� [piksele na sekund�]
	float movementSpeed;
};

