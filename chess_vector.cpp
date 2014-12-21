#include "chess_vector.h"

float & Vector::operator[](const long idx)
{
    return *((&x) + idx);
}

const Vector& Vector::operator=(const Vector& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;

    return *this;
}

const bool Vector::operator==(const Vector& vec) const
{
    return ((x == vec.x) && (y == vec.y) && (z == vec.z));
}

const bool Vector::operator!=(const Vector& vec) const
{
    return !(*this == vec);
}

const Vector Vector::operator+(const Vector& vec) const
{
    return Vector(x + vec.x, y + vec.y, z + vec.z);
}

const Vector Vector::operator+() const
{
    return Vector(*this);
}

const Vector& Vector::operator+=(const Vector& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;

    return *this;
}

const Vector Vector::operator-(const Vector& vec) const
{
    return Vector(x - vec.x, y - vec.y, z - vec.z);
}

const Vector Vector::operator-() const
{
    return Vector(-x, -y, -z);
}

const Vector& Vector::operator-=(const Vector& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;

    return *this;
}

const Vector& Vector::operator*=(const float& s)
{
    x *= s;
    y *= s;
    z *= s;

    return *this;
}

const Vector& Vector::operator/=(const float& s)
{
    const float recip = 1/s; // for speed, one division

    x *= recip;
    y *= recip;
    z *= recip;

    return *this;
}

const Vector Vector::operator*(const float& s) const
{
    return Vector(x*s, y*s, z*s);
}


const Vector Vector::operator*(const Vector& vec) const
{
    return Vector(x*vec.x, y*vec.y, z*vec.z);
}

const Vector Vector::operator/(float s) const
{
    s = 1/s;

    return Vector(s*x, s*y, s*z);
}

const Vector Vector::cross(const Vector& vec) const
{
    return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
}

const Vector Vector::operator^(const Vector& vec) const
{
    return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
}

const float Vector::dot(const Vector& vec) const
{
    return x*vec.x + y*vec.x + z*vec.z;
}

const float Vector::operator%(const Vector& vec) const
{
    return x*vec.x + y*vec.x + z*vec.z;
}

const float Vector::length() const
{
    return (float)sqrt((double)(x*x + y*y + z*z));
}

const Vector Vector::unit() const
{
    return (*this) / length();
}

void Vector::normalize()
{
    (*this) /= length();
}

const float Vector::operator!() const
{
    return sqrtf(x*x + y*y + z*z);
}

const Vector Vector::operator|(const float length) const
{
    return *this * (length / !(*this));
}

const Vector& Vector::operator|=(const float length)
{
    return *this = *this | length;
}

