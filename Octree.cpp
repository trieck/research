/////////////////////////////////////////////////////////////////////////////
// 
// OCTREE.CPP : Octree data structure
// Each node in an octree subdivides the space it represents into eight 
// octants. The subdivision point is implicitly the center of the space the 
// node represents. The root node of an octree must represent a finite 
// bounded space so that the implicit centers are well-defined.
//

#include "stdafx.h"
#include "Octree.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
Octree::Octree(const Vector &origin, const Vector& halfDims) : data(NULL)
{
	this->origin = origin;
	this->halfDims = halfDims;
	memset(children, 0, sizeof (Octree*) * 8);
}

/////////////////////////////////////////////////////////////////////////////
Octree::~Octree() 
{
	destroy();
}

/////////////////////////////////////////////////////////////////////////////
void Octree::destroy()
{
	if (data != NULL) {
		delete data;
		data = NULL;
	}

	for (int i = 0; i < 8; ++i)
		delete children[i];
}

/////////////////////////////////////////////////////////////////////////////
bool Octree::isLeaf() const
{
	return children[0] == NULL;
}

/////////////////////////////////////////////////////////////////////////////
int Octree::octant(const Datum& datum) const 
{
	int octant = 0;

	const Vector& point = datum.getPosition();

	if (point.x >= origin.x) octant |= 4;
	if (point.y >= origin.y) octant |= 2;
	if (point.z >= origin.z) octant |= 1;

	return octant;
}

/////////////////////////////////////////////////////////////////////////////
void Octree::insert(const Datum& d) 
{
	if (isLeaf()) {
		if (data == NULL) {
			data = new Datum(d);
			return;
		}

		// retain original value
		Datum* save = data;
		data = NULL;

		// split the tree
		for (int i = 0; i < 8; ++i) {
			Vector newOrigin = origin;
			newOrigin.x += halfDims.x * (i & 4 ? .5f : -.5f);
			newOrigin.y += halfDims.y * (i & 2 ? .5f : -.5f);
			newOrigin.z += halfDims.z * (i & 1 ? .5f : -.5f);
			children[i] = new Octree(newOrigin, halfDims * .5f);
		}

		// re-insert original value
		children[octant(*save)]->insert(*save);
	} 
	children[octant(d)]->insert(d);
}

/////////////////////////////////////////////////////////////////////////////
void Octree::query(const Region &region, DatumVec& results)
{
	if (isLeaf()) {
		if (data != NULL) {
			const Vector& v = data->getPosition();
			if (region.contains(v)) results.push_back(*data);
		}
	} else {
		for (int i = 0; i < 8; ++i) {
			// compute min/max corners of the octant
			Vector cmin = children[i]->origin - children[i]->halfDims;
			Vector cmax = children[i]->origin + children[i]->halfDims;

			// check if query intersects child's bounding region
			Region childRegion(cmin, cmax);
			if (region.intersects(childRegion))
				children[i]->query(region, results);
		}
	}
}