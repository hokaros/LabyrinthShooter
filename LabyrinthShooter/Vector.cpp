#include "Vector.h"

Vector::Vector()
	: x(0), y(0) {

}

Vector::Vector(float x, float y)
	: x(x), y(y) {

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
