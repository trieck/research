/////////////////////////////////////////////////////////////////////////////
// 
// DATUM.CPP : Octree datum 
//

#include "stdafx.h"
#include "Datum.h"

/////////////////////////////////////////////////////////////////////////////
Datum::Datum(const Vector& pos) 
{
	position = pos;
}

/////////////////////////////////////////////////////////////////////////////
Datum::Datum(const Datum& datum)
{
	*this = datum;
}

/////////////////////////////////////////////////////////////////////////////
Datum::~Datum()
{
}

/////////////////////////////////////////////////////////////////////////////
Datum& Datum::operator = (const Datum& rhs)
{
	if (this != &rhs) {
		position = rhs.position;
	}

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
const Vector& Datum::getPosition() const
{
	return position;
}
