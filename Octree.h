#ifndef OCTREE_H
#define	OCTREE_H

#include "Vector.h"

class Octree {
public:
  Octree(const Vector &origin, float size);
  virtual ~Octree();
  
  void insert(const Vector& v);
  bool isLeaf() const;
  
private:
  void destroy();
  Octree *children[8];
  Vector origin;
  float size;
};

#endif  // OCTREE_H


