/////////////////////////////////////////////////////////////////////////////
// 
// REGION.H : Bounding region class
//

#ifndef __REGION_H__
#define	__REGION_H__

#include "Vector.h"

/////////////////////////////////////////////////////////////////////////////
class Region
{
    // Construction / Destruction
public:
    Region(const Vector& min, const Vector& max);
    Region(const Region& region);
    ~Region();

    // Interface
    Region& operator =(const Region& region);
    bool contains(const Vector& point) const;
    bool intersects(const Region& rgn) const;

    // Implementation
private:
    Vector min, max;
};

#endif  // __REGION_H__
