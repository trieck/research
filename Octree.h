#ifndef OCTREE_H
#define	OCTREE_H

#include "Vector.h"

class Octree {
public:
  Octree();
  Octree(const Octree& orig);
  virtual ~Octree();
  
  void insert(const Vector& v);
  bool isLeaf() const;
  
private:
  void destroy();
  Octree *children[8];
  Vector origin;
  float extent;
};

#endif  // OCTREE_H


