#pragma once
#include "ObjectComponent.h"
#include "Firearm.h"

enum class WeaponType { Basic, Super };

// Komponent odpowiedzialny za zmienianie broni
class PlayerEquipment :
	public ObjectComponent
{
public:
	PlayerEquipment(GameObject& owner);

	void Start() override;

	void EquipWeapon(WeaponType weaponType);
	void SwitchWeapon();

	Firearm* GetCurrentWeapon();

	ObjectComponent* Copy(GameObject& newOwner) override;

private:
	Firearm* basicWpn = NULL;
	Firearm* superWpn = NULL;

	Firearm* currWpn = NULL;

private:
	void EquipWeapon(Firearm* wpn);
	void LoadWeaponsFromChildren();
};

