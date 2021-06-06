#include "ColliderMemory.h"

ColliderMemory::ColliderMemory(size_t width, size_t height)
	: width(width), height(height) {

	memory = new bool* [width];

	for (size_t x = 0; x < width; x++) {
		memory[x] = new bool[height];

		for (size_t y = 0; y < height; y++) {
			memory[x][y] = false;
		}
	}
}

ColliderMemory::~ColliderMemory() {
	for (size_t x = 0; x < width; x++) {
			delete[] memory[x];
	}

	delete[] memory;
}

void ColliderMemory::Refresh(GameObject** objects, size_t count) {
	Clear();

	for (size_t i = 0; i < count; i++) {
		Claim(objects[i]);
	}
}

void ColliderMemory::Clear() {
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			memory[x][y] = false;
		}
	}
}

void ColliderMemory::Claim(GameObject* collider) {
	SetForCollider(collider, true);
}


void ColliderMemory::Free(GameObject* collider) {
	SetForCollider(collider, false);
}

void ColliderMemory::SetForCollider(GameObject* collider, bool occupied) {
	std::vector<VectorInt>* pixels = collider->GetPixels();

	for (VectorInt pixel : *pixels) {
		if (pixel.x >= 0 && pixel.x < width
			&& pixel.y >= 0 && pixel.y < height) {

			memory[pixel.x][pixel.y] = occupied;
		}
	}
}

bool ColliderMemory::IsOccupied(const VectorInt& point) const {
	if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height)
		return false;

	return memory[point.x][point.y];
}

size_t ColliderMemory::GetWidth() const {
	return width;
}

size_t ColliderMemory::GetHeight() const {
	return height;
}