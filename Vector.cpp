#include "stdafx.h"
#include "Vector.h"

Vector::Vector() {
  x = y = z = 0;
}

Vector::Vector(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector::Vector(const Vector& orig) {
  *this = orig;
}

Vector::~Vector() {
}

Vector& Vector::operator =(const Vector& rhs) {
  if (this != &rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
  }

  return *this;
}

bool operator == (const Vector& lhs, const Vector &rhs) {
  return (lhs.x == rhs.x ) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}
