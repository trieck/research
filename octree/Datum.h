/////////////////////////////////////////////////////////////////////////////
// 
// DATUM.H : Octree datum 
//

#ifndef __DATUM_H__
#define	__DATUM_H__

#include "Vector.h"

/////////////////////////////////////////////////////////////////////////////
class Datum 
{
	// Construction / Destruction
public:
  Datum(const Vector& position);
	Datum(const Datum& rhs);
  ~Datum();

	// Interface
	Datum& operator = (const Datum& rhs);
	const Vector &getPosition() const;

	// Implementation
private:
	Vector position;
};

typedef std::vector<Datum> DatumVec;

#endif  // __DATUM_H__


