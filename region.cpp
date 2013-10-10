/////////////////////////////////////////////////////////////////////////////
// 
// REGION.CPP : Bounding region class for querying octree
//
#include "stdafx.h"
#include "region.h"

/////////////////////////////////////////////////////////////////////////////
Region::Region(const Vector &min, const Vector& max)
{
	this->min = min;
	this->max = max;
}

/////////////////////////////////////////////////////////////////////////////
Region::~Region()
{
}

/////////////////////////////////////////////////////////////////////////////
bool Region::contains(const Vector& point) const
{
	if (point.x > max.x || point.y > max.y || point.z > max.z)
		return false;

	if (point.x < min.x || point.y < min.y || point.z < min.z)
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool Region::intersects(const Region& rgn) const
{
	if (rgn.max.x < min.x || rgn.max.y < min.y || rgn.max.z < min.z) 
		return false;

	if (rgn.min.x > max.x || rgn.min.y > max.y || rgn.min.z > max.z)
		return false;

	return true;
}
