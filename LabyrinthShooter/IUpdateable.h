#pragma once

class IUpdateable {
public:
	// Wywoływane raz na klatkę
	virtual void Update() = 0;

	// Wywoływane przed rozpoczęciem gry
	virtual void Start() {}

	virtual IUpdateable* Copy() = 0;
};