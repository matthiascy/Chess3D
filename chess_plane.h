/*
 * @file:        chess_plane.h
 * @author:      matthiascy(matthias_cy@outlook.com)
 * @date:        2014-12-20
 * @description: 
 */

#ifndef __CHESS_PLANE_H__
#define __CHESS_PLANE_H__

#include "chess_vector.h"

class Plane {
public:
  Vector N;    // plane normal
  float D;       // plane shift constant

  // Ax + By + Cz - D = 0
  Plane(float a = 1, float b = 0, float c = 0, float d = 0);

  // instance a plane with normal and d
  Plane(const Vector& normal, float d = 0);

  // instance a copy of plane
  Plane(const Plane& plane);

  // instance a plane with three points
  Plane(const Vector& vertexA, const Vector& vertexB, const Vector& vertexC);

  // assignment operator
  const Plane& operator =(const Plane& plane);

  // equality operator
  const bool operator ==(const Plane& plane) const;

  // inequality operator
  const bool operator !=(const Plane& plane) const;

  // is point on this plane?
  const bool inline pointOnPlane(const Vector& point) const;

  // return the distance of point to the plane
  const float inline distanceToPlane(const Vector& point) const;

  // return the intersection point of the ray to this plane
  const Vector inline rayIntersection(const Vector& rayPos,
    const Vector& rayDir) const;

};

const bool Plane::pointOnPlane(const Vector& point) const
{
  return distanceToPlane(point) == 0;
}

const float Plane::distanceToPlane(const Vector& point) const
{
  return N % point + D;
}

const Vector Plane::rayIntersection(const Vector& rayPos, const Vector& rayDir) const
{
  const float a = N % rayDir;

  if (a == 0) return rayPos; // ray is parallel to plane

  return rayPos - rayDir * (distanceToPlane(rayPos) / a);
}

#endif // __CHESS_PLANE_H__