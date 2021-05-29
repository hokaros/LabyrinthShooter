#pragma once
#include "ObjectComponent.h"
#include "InputController.h"
#include "PlayerEquipment.h"

#define WPN_SWITCH_KEY SDLK_q

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

