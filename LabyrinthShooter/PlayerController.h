#pragma once
#include "ObjectComponent.h"
#include "InputController.h"
#include "PlayerEquipment.h"
#include "Firearm.h"

#define WPN_SWITCH_KEY SDLK_q
#define SHOOT_KEY SDLK_SPACE

class PlayerController :
	public ObjectComponent
{
public:
	PlayerController(GameObject& player, float movementSpeed);
	void Update() override;
	void Start() override;

	ObjectComponent* Copy(GameObject& newOwner) override;
protected:
	// Pr�dko�� [piksele na sekund�]
	float movementSpeed;

	InputController* input = NULL;
	PlayerEquipment* equipment = NULL;

private:
	void ProcessMovement();
	void ProcessAim();
};

