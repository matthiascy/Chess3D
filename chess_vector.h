#ifndef __CHESS_VECTOR_H__
#define __CHESS_VECTOR_H__

#include <math.h>

#define PI        3.14159265359f
#define DEG2RAD(a)    PI/180*a
#define RAD2DEG(a)    180/PI*a

typedef float scalar_t;

class Vector {
public:
  union {
    struct {
      scalar_t x;
      scalar_t y;
      scalar_t z;
    };
    scalar_t v[3];
  };

public :
  Vector(scalar_t a = 0, scalar_t b = 0, scalar_t c = 0): x(a),
    y(b), z(c) {};
  Vector(const Vector& vec): x(vec.x), y(vec.y), z(vec.z) {};

  // vector index
  scalar_t &operator [](const long idx);

  // vector assignment
  const Vector& operator =(const Vector& vec);

  // vector equality
  const bool operator ==(const Vector& vec) const;

  // vector inequality
  const bool operator !=(const Vector& vec) const;

  // vector add
  const Vector operator +(const Vector& vec) const;

  // vector add (opposite of negation)
  const Vector operator +() const;

  // vector increment
  const Vector& operator +=(const Vector& vec);

  // vector subtraction
  const Vector operator -(const Vector& vec) const;

  // vector negation
  const Vector operator -() const;

  // vector decrement
  const Vector& operator -=(const Vector& vec);

  // scalar self-multiply
  const Vector& operator *=(const scalar_t& s);

  // scalar self-divide
  const Vector& operator /=(const scalar_t& s);

  // right multiply by scalar
  const Vector operator *(const scalar_t& s) const;

  // left multiply by scalar
  friend inline const Vector operator *(const scalar_t& s, const Vector& vec) {
    return vec*s;
  }

  // multiply by vector
  const Vector operator *(const Vector& vec) const;

  // divide by scalar
  const Vector operator /(scalar_t s) const;

  // cross product
  const Vector cross(const Vector& vec) const;

  // cross product
  const Vector operator ^(const Vector& vec) const;

  // dot product
  const scalar_t dot(const Vector& vec) const;

  // dot product
  const scalar_t operator %(const Vector& vec) const;

  // length of vector
  const scalar_t length() const;

  // return unit vector
  const Vector unit() const;

  // normalize this vector
  void normalize();

  // square root
  const scalar_t operator !() const;

  // return vector with specified length
  const Vector operator |(const scalar_t length) const;

  // set length of vector equal to length
  const Vector& operator |=(const scalar_t length);

  // return angle between two vectors
  const scalar_t inline angle(const Vector& normal) const;

  // reflect this vector off surface with normal vector
  const Vector inline reflect(const Vector& normal) const;

  // rotate angle degrees about a normal
  const Vector inline rotate(const scalar_t angle, const Vector& normal) const;

};

const scalar_t Vector::angle(const Vector& normal) const
{
  return acosf(*this % normal);
}

const Vector Vector::reflect(const Vector& normal) const
{
  const Vector vec(*this | 1);  // normalize this vector
  return (vec - normal * 2.0 * (vec % normal)) * !*this;
}

const Vector Vector::rotate(const scalar_t angle, const Vector& normal) const
{
  const float cosine = cosf(angle);
  const float sine = sinf(angle);

  return Vector(*this * cosine + ((normal * *this) * (1.0f - cosine)) *
    normal + (*this ^ normal) * sine);
}


#endif // __CHESS_VECTOR_H__