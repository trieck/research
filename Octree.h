/////////////////////////////////////////////////////////////////////////////
// 
// OCTREE.H : Octree data structure
// Each node in an octree subdivides the space it represents into eight 
// octants. The subdivision point is implicitly the center of the space the 
// node represents. The root node of an octree must represent a finite 
// bounded space so that the implicit centers are well-defined.
//

#ifndef __OCTREE_H__
#define	__OCTREE_H__

#include "Vector.h"

/////////////////////////////////////////////////////////////////////////////
class Octree 
{
	// Construction / Destruction
public:
  Octree(const Vector &origin, float size);
  virtual ~Octree();

	// Interface
  void insert(const Vector& point);
  bool isLeaf() const;
  int octant(const Vector& point) const;

	// Implementation
private:
  void destroy();
  Octree *children[8];  // children
  Vector origin;        // origin
  Vector *value;        // leaf value
  float size;           // dimensions
};

#endif  // __OCTREE_H__


