#include "Octree.h"
#include <string.h>

Octree::Octree(const Vector &origin, float size)
{
  this->origin = origin;
  this->size = size;  
  memset(children, 0, sizeof (Octree*) * 8);
}
  
Octree::~Octree() {
  destroy();
}

void Octree::destroy() {
  for (int i = 0; i < 8; ++i)
    delete children[i];
}

bool Octree::isLeaf() const {
  return children[0] == NULL;
}

void Octree::insert(const Vector& v) {
  
  // split the current node and make new trees for each child octant
  for (int i = 0; i < 8; ++i) {
    Vector newOrigin = origin;
    newOrigin.z += size * (i & 1 ? .5 : -.5); // ??? all odd nodes have +z?
    
    
  }
  
  /*
    // Split the current node and create new empty trees for each
          // child octant.
          for(int i=0; i<8; ++i) {
            // Compute new bounding box for this child
            Vec3 newOrigin = origin;
            newOrigin.x += halfDimension.x * (i&4 ? .5f : -.5f);
            newOrigin.y += halfDimension.y * (i&2 ? .5f : -.5f);
            newOrigin.z += halfDimension.z * (i&1 ? .5f : -.5f);
            children[i] = new Octree(newOrigin, halfDimension*.5f);
          }*
  l */

}