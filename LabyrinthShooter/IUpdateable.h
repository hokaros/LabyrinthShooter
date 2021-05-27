#pragma once

class IUpdateable {
public:
	// Wywo³ywane raz na klatkê
	virtual void Update() = 0;

	virtual IUpdateable* Copy() = 0;
};