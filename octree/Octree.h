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

typedef std::stack<uint8_t> ByteStack;

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
	void query(const wstring& q, DatumVec& results) const;
	bool contains(const Vector& point) const;
	wstring encode(const Vector& point) const;
	Region getRegion() const;
	void Walk(PWALKER);

	// Implementation
private:
	bool isLeaf() const;
	uint8_t octant(const Datum& d) const;
	uint8_t octant(const Vector& d) const;
	bool encodeR(const Vector& point, ByteStack& stack) const;
	wchar_t encodeOctant(uint8_t octant) const;
	void query(const wstring& q, uint32_t index, DatumVec& results) const;
	void destroy();
	Octree*children[8];	// children
	Vector origin;				// origin
	Vector halfDims;			// half-dimension
	Datum *data;					// leaf data
};

#endif  // __OCTREE_H__


