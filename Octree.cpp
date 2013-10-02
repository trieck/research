#include "stdafx.h"
#include "Octree.h"
#include <string.h>

Octree::Octree(const Vector &origin, float size) : value(0) {
  this->origin = origin;
  this->size = size;
  memset(children, 0, sizeof (Octree*) * 8);
}

Octree::~Octree() {
  destroy();
}

void Octree::destroy() {
  if (value != NULL) {
    delete value;
    value = NULL;
  }

  for (int i = 0; i < 8; ++i)
    delete children[i];
}

bool Octree::isLeaf() const {
  return children[0] == NULL;
}

int Octree::octant(const Vector& point) const {

  int octant = 0;

  if (point.x >= origin.x) octant |= 4;
  if (point.y >= origin.y) octant |= 2;
  if (point.z >= origin.z) octant |= 1;

  return octant;
}

void Octree::insert(const Vector& point) {

  if (isLeaf()) {
    if (value == NULL) {
      value = new Vector(point);
      return;
    }

    // retain original value
    Vector* save = value;
    value = NULL;

    for (int i = 0; i < 8; ++i) {
      Vector newOrigin = origin;
      newOrigin.x += size * (i & 4 ? .5f : -.5f);
      newOrigin.y += size * (i & 2 ? .5f : -.5f);
      newOrigin.z += size * (i & 1 ? .5f : -.5f);

      children[i] = new Octree(newOrigin, size * .5f);
    }
    
    // re-insert original value
    children[octant(*save)]->insert(*save);
  } 
  children[octant(point)]->insert(point);
}