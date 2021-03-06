#pragma once
#include "Shared.h"

class IUpdateable {
public:
	// Wywoływane raz na klatkę
	virtual void Update() {}
	virtual void RenderUpdate() {}

	// Wywoływane przed rozpoczęciem gry
	virtual void Start() {}

	virtual IUpdateable* Copy() = 0;
};