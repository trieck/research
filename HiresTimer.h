/////////////////////////////////////////////////////////////////////////////
//
//	HIRESTIMER.H : High resolution timer
//
//	Copyright(c) 2006, Thomas A. Rieck, All Rights Reserved
//

#ifndef __HIRESTIMER_H__
#define __HIRESTIMER_H__

/////////////////////////////////////////////////////////////////////////////
class HiresTimer {
public:
	HiresTimer();
	~HiresTimer();

	operator string() const;
private:
	INT64 LI2INT64(PLARGE_INTEGER li) const;

	LARGE_INTEGER start;
	INT64 ticksPerSecond;
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
ostream & operator << (ostream &os, const HiresTimer &t);

#endif // __HIRESTIMER_H__