/////////////////////////////////////////////////////////////////////////////
//
//	HIRESTIMER.H : High resolution timer
//

#ifndef __HIRESTIMER_H__
#define __HIRESTIMER_H__

/////////////////////////////////////////////////////////////////////////////
class HiresTimer
{
public:
	HiresTimer();
	~HiresTimer();

	void reset();
	uint64_t millis() const;
	operator wstring() const;
private:
	INT64 LI2INT64(PLARGE_INTEGER li) const;

	LARGE_INTEGER start;
	INT64 ticksPerSecond;
};

/////////////////////////////////////////////////////////////////////////////
inline wostream& operator << (wostream& s, HiresTimer& timer)
{
    return s << wstring(timer);
}

#endif // __HIRESTIMER_H__