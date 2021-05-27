#pragma once

class IUpdateable {
public:
	// Wywo�ywane raz na klatk�
	virtual void Update() = 0;

	virtual IUpdateable* Copy() = 0;
};