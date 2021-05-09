#include "GameObject.h"

GameObject::GameObject()
	: size(1, 1) {

}

GameObject::GameObject(const Vector& size)
	: size(size) {

}

GameObject::GameObject(const Vector& size, const Vector& position)
	: position(position), size(size) {

}

GameObject::~GameObject() {
	for (IUpdateable* component : components) {
		delete component;
	}
}

void GameObject::AddComponent(IUpdateable* component) {
	components.push_back(component);
}

void GameObject::Update() {
	for (IUpdateable* component : components) {
		component->Update();
	}
}

Vector GameObject::GetSize() const {
	return size;
}