#include "chess_plane.h"


Plane::Plane(float a /*= 1*/, float b /*= 0*/, float c /*= 0*/,
             float d /*= 0*/)
{
    N = Vector(a, b, c);
    D = d;
}

Plane::Plane(const Vector& normal, float d /*= 0*/)
{
    N = normal;
    D = d;
}

Plane::Plane(const Plane& plane)
{
    N = plane.N;
    D = plane.D;
}

Plane::Plane(const Vector& vertexA, const Vector& vertexB, const Vector& vertexC)
{
    Vector normalA((vertexC - vertexA) | 1.0); // unit normal of C - A
    Vector normalB((vertexC - vertexB) | 1.0); // unit normal of C - B

    N = (normalA ^ normalB) | 1.0;  // normalize cross product
    D = -vertexA % N;   // calculate distance
}

const Plane& Plane::operator=(const Plane& plane)
{
    N = plane.N;
    D = plane.D;

    return *this;
}

const bool Plane::operator==(const Plane& plane) const
{
    return N == plane.N && D == plane.D;
}

const bool Plane::operator!=(const Plane& plane) const
{
    return !(*this == plane);
}
