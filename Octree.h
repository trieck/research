#ifndef OCTREE_H
#define	OCTREE_H

#include "Vector.h"

class Octree {
public:
  Octree(const Vector &origin, float size);
  virtual ~Octree();

  void insert(const Vector& point);
  bool isLeaf() const;
  int octant(const Vector& point) const;

private:
  void destroy();
  Octree *children[8];
  Vector origin;
  float size;
};

#endif  // OCTREE_H


