#pragma once

class IUpdateable {
public:
	// Wywo�ywane raz na klatk�
	virtual void Update() { }

	// Wywo�ywane przed rozpocz�ciem gry
	virtual void Start() {}

	virtual IUpdateable* Copy() = 0;
};