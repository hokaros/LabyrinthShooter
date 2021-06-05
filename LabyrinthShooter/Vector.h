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
    // Sprowadza do d�ugo�ci 1
    void Normalize();
    // Sprawia, �e x <= y
    void Sort();
    float Length() const;
    // Obraca o k�t w radianach
    void Rotate(float angle);
    // Zwraca k�t w radianach wskazywany przez wektor
    double GetAngle() const;

    friend Vector operator+(const Vector& left, const Vector& right);
    friend Vector& operator+=(Vector& left, const Vector& right);
    friend Vector operator-(const Vector& left, const Vector& right);
    friend Vector operator*(const Vector& left, float multiplier);
    friend Vector operator/(const Vector& left, float dividor);

    // K�t w radianach mi�dzy dwoma wektorami
    static double GetAngle(const Vector& v1, const Vector& v2);
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
