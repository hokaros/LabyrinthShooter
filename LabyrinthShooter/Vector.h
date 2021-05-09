#pragma once
#include <math.h>

struct Vector
{
public:
    float x;
    float y;
public:
    Vector();
    Vector(float x, float y);
    // Sprowadza do d³ugoœci 1
    void Normalize();

    friend Vector operator+(const Vector& left, const Vector& right);
    friend Vector& operator+=(Vector& left, const Vector& right);
    friend Vector operator*(const Vector& left, float multiplier);
};

