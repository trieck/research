#ifndef VECTOR_H
#define	VECTOR_H

class Vector {
public:
  Vector();
  Vector(float x, float y, float z);
  Vector(const Vector& orig);
  virtual ~Vector();

  Vector& operator =(const Vector& rhs);

  float x, y, z;
  friend bool operator ==(const Vector& lhs, const Vector &rhs);
};

#endif	/* VECTOR_H */

