#pragma once
#include <math.h>

enum class Direction { NORTH, SOUTH, EAST, WEST };

struct Vector;

struct VectorInt
{
public:
    int x;
    int y;
public:
    VectorInt();
    VectorInt(int x, int y);
    VectorInt(const Vector& other);

    friend VectorInt operator+(const VectorInt& left, const VectorInt& right);
    friend VectorInt& operator+=(VectorInt& left, const VectorInt& right);
    friend VectorInt operator*(const VectorInt& left, int multiplier);
};

struct Vector
{
public:
    float x;
    float y;
public:
    Vector();
    Vector(float x, float y);
    Vector(Direction direction);
    Vector(const VectorInt& other);
    // Sprowadza do d³ugoœci 1
    void Normalize();
    // Sprawia, ¿e x <= y
    void Sort();
    float Length() const;

    friend Vector operator+(const Vector& left, const Vector& right);
    friend Vector& operator+=(Vector& left, const Vector& right);
    friend Vector operator-(const Vector& left, const Vector& right);
    friend Vector operator*(const Vector& left, float multiplier);
    friend Vector operator/(const Vector& left, float dividor);
};

struct Rect 
{
public:
    Vector pos;
    Vector size;
public:
    Rect(const Vector& position, const Vector& size);

    Vector GetMiddle() const;
};
