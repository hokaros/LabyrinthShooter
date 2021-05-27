#include "ObjectComponent.h"
ObjectComponent::ObjectComponent(GameObject& owner)
	: gameObject(owner) {

}

IUpdateable* ObjectComponent::Copy() {
	return Copy(gameObject);
}