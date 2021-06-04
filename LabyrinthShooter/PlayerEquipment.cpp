#include "PlayerEquipment.h"

PlayerEquipment::PlayerEquipment(GameObject& owner, GameObject* basicWeapon, GameObject* superWeapon)
	: ObjectComponent(owner), basicWpn(basicWeapon), superWpn(superWeapon) {
	EquipWeapon(basicWeapon);

	if (superWeapon != NULL) {
		superWeapon->SetEnabled(false);
	}
}

ObjectComponent* PlayerEquipment::Copy(GameObject& newOwner) {
	return new PlayerEquipment(newOwner, basicWpn, superWpn);
}

void PlayerEquipment::EquipWeapon(WeaponType weaponType) {
	if (weaponType == WeaponType::Basic) {
		EquipWeapon(basicWpn);
	}
	else {
		EquipWeapon(superWpn);
	}
}

void PlayerEquipment::SwitchWeapon() {
	if (currWpn == basicWpn)
		EquipWeapon(superWpn);
	else
		EquipWeapon(basicWpn);
}

void PlayerEquipment::EquipWeapon(GameObject* wpn) {
	if (currWpn != NULL) {
		currWpn->SetEnabled(false);
	}
	if (wpn != NULL) {
		wpn->SetEnabled(true);
	}

	currWpn = wpn;
}

Firearm* PlayerEquipment::GetCurrentWeapon() {
	if (currWpn == NULL)
		return NULL;

	return currWpn->FindComponent<Firearm>();
}