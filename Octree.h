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

#include "Datum.h"
#include "region.h"

typedef std::stack<string> StringStack;

/////////////////////////////////////////////////////////////////////////////
class Octree : public IWalkable<Datum> 
{
	// Construction / Destruction
public:
	Octree(const Vector& origin, const Vector& halfDims);
	virtual ~Octree();

	// Interface
	void insert(const Datum& d);
	void query(const Region& region, DatumVec& results) const;
	bool contains(const Vector& point) const;
	string encode(const Vector& point) const;
	void Walk(PWALKER);

	// Implementation
private:
	bool isLeaf() const;
	int octant(const Datum& d) const;
	int octant(const Vector& d) const;
	void corners(Vector& min, Vector& max) const;
	bool encodeR(const Vector& point, StringStack& stack) const;
	string encodeOctant(const Vector& point) const;
	void destroy();
	Octree *children[8];	// children
	Vector origin;				// origin
	Vector halfDims;			// half-dimension
	Datum *data;					// leaf data
};

#endif  // __OCTREE_H__


