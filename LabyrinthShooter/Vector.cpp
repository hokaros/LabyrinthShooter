#include "Vector.h"

Vector::Vector()
	: x(0), y(0) {

}

Vector::Vector(float x, float y)
	: x(x), y(y) {

}

Vector::Vector(const VectorInt& other)
	: x(other.x), y(other.y) {

}

Vector::Vector(Direction direction) : x(0), y(0) {
	switch (direction) {
	case Direction::EAST:
		x = -1;
		y = 0;
		break;
	case Direction::WEST:
		x = 1;
		y = 0;
		break;
	case Direction::NORTH:
		x = 0;
		y = -1;
		break;
	case Direction::SOUTH:
		x = 0;
		y = 1;
		break;
	}
}

void Vector::Normalize() {
	float length = sqrtf(x * x + y * y);
	if (length == 0)
		return;

	x /= length;
	y /= length;
}


Vector operator+(const Vector& left, const Vector& right) {
	Vector result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

Vector operator+(const Vector& left, const VectorInt& right) {
	Vector result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

Vector& operator+=(Vector& left, const Vector& right) {
	left.x += right.x;
	left.y += right.y;

	return left;
}

Vector operator*(const Vector& left, float multiplier) {
	Vector result;
	result.x = left.x * multiplier;
	result.y = left.y * multiplier;

	return result;
}



VectorInt::VectorInt()
	: x(0), y(0) {

}

VectorInt::VectorInt(int x, int y)
	: x(x), y(y) {

}

VectorInt::VectorInt(const Vector& other)
	: x(other.x), y(other.y) {

}

VectorInt operator+(const VectorInt& left, const VectorInt& right) {
	VectorInt result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

VectorInt& operator+=(VectorInt& left, const VectorInt& right) {
	left.x += right.x;
	left.y += right.y;

	return left;
}

VectorInt operator*(const VectorInt& left, int multiplier) {
	VectorInt result;
	result.x = left.x * multiplier;
	result.y = left.y * multiplier;

	return result;
}

Vector operator*(const VectorInt& left, float multiplier) {
	Vector result;
	result.x = left.x * multiplier;
	result.y = left.y * multiplier;

	return result;
 }