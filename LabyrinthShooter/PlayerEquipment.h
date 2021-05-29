#pragma once
#include "ObjectComponent.h"
#include "Firearm.h"

enum class WeaponType { Basic, Super };

// Komponent odpowiedzialny za zmienianie broni
class PlayerEquipment :
	public ObjectComponent
{
public:
	PlayerEquipment(GameObject& owner, GameObject* basicWeapon, GameObject* superWeapon);

	void EquipWeapon(WeaponType weaponType);
	void SwitchWeapon();

	virtual void Update() override;

	ObjectComponent* Copy(GameObject& newOwner) override;

private:
	GameObject* basicWpn;
	GameObject* superWpn;

	GameObject* currWpn;

private:
	void EquipWeapon(GameObject* wpn);
};

