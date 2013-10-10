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
	return octant(datum.getPosition());
}

/////////////////////////////////////////////////////////////////////////////
int Octree::octant(const Vector& point) const 
{
	int octant = 0;

	if (point.x >= origin.x) octant |= 4;
	if (point.y >= origin.y) octant |= 2;
	if (point.z >= origin.z) octant |= 1;

	return octant;
}

/////////////////////////////////////////////////////////////////////////////
wstring Octree::encodeOctant(const Vector& point) const
{
	wstring output;

	uint8_t o = octant(point) & 7;

	for (int i = 2; i >= 0; i--) {
		output += o & (1 << i) ? L'1' : L'0';
	}

	return output;
}

/////////////////////////////////////////////////////////////////////////////
void Octree::corners(Vector& min, Vector& max) const
{
	// compute min/max corners of the octant
	min = origin - halfDims;
	max = origin + halfDims;
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
void Octree::query(const Region &region, DatumVec& results) const
{
	if (isLeaf()) {
		if (data != NULL) {
			const Vector& v = data->getPosition();
			if (region.contains(v)) results.push_back(*data);
		}
	} else {
		for (int i = 0; i < 8; ++i) {
			Vector cmin, cmax;
			children[i]->corners(cmin, cmax);

			// check if query intersects child's bounding region
			Region childRegion(cmin, cmax);
			if (region.intersects(childRegion))
				children[i]->query(region, results);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
bool Octree::contains(const Vector& point) const
{
	if (isLeaf()) {
		if (data != NULL) {
			return point == data->getPosition();
		}
	} else {
		for (int i = 0; i < 8; ++i) {
			Vector cmin, cmax;
			children[i]->corners(cmin, cmax);

			// check if point is contained in child's bounding region
			Region childRegion(cmin, cmax);
			if (childRegion.contains(point))
				if (children[i]->contains(point))
					return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
wstring Octree::encode(const Vector& point) const
{
	StringStack stack;

	if (!encodeR(point, stack))
		return L"";

	wstring output;
	while (!stack.empty()) {
		output += stack.top();
		stack.pop();
	}

	return output;
}

/////////////////////////////////////////////////////////////////////////////
bool Octree::encodeR(const Vector& point, StringStack& stack) const
{
	if (isLeaf()) {
		if (data != NULL) {
			if (point == data->getPosition()) {
				stack.push(L"000");
				return true;
			}
		}
	} else {
		for (int i = 0; i < 8; ++i) {
			Vector cmin, cmax;
			children[i]->corners(cmin, cmax);

			// check if point is contained in child's bounding region
			Region childRegion(cmin, cmax);
			if (childRegion.contains(point)) {
				if (children[i]->encodeR(point, stack)) {
					stack.push(encodeOctant(point));
					return true;
				}
			}
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
void Octree::Walk(PWALKER pWalker)
{
	if (isLeaf()) {
		if (data != NULL) {
			pWalker->Call(data);
		}
	} else {
		for (int i = 0; i < 8; ++i) {
			children[i]->Walk(pWalker);
		}
	}
}