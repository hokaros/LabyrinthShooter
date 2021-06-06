#pragma once
#include "GameObject.h"

class ColliderMemory
{
public:
	ColliderMemory(size_t width, size_t height);
	~ColliderMemory();

	void Refresh(GameObject** objects, size_t count);
	// Ustawia po�o�enie collidera jako zaj�te
	void Claim(GameObject* collider);
	// Zwalnia po�o�enie collidera
	void Free(GameObject* collider);

	bool IsOccupied(const VectorInt& point) const;
	// Czy jakiekolwiek pole jest zaj�te na linii od start do end
	bool Raycast(const VectorInt& start, const VectorInt& end, GameObject* ignore = NULL) const;
	size_t GetWidth() const;
	size_t GetHeight() const;

private:
	GameObject*** memory;
	size_t width;
	size_t height;

private:
	void Clear();
	void SetForCollider(GameObject* collider, GameObject* occupier);
};

