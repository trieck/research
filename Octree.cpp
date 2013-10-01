#include "Octree.h"
#include <string.h>

Octree::Octree() : extent(0) {
  memset(children, 0, sizeof (Octree*) * 8);
}

Octree::Octree(const Octree& orig) {
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