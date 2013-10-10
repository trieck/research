/////////////////////////////////////////////////////////////////////////////
//
//	HIRESTIMER.CPP : High resolution timer
//

#include "common.h"
#include "HiresTimer.h"

/////////////////////////////////////////////////////////////////////////////
HiresTimer::HiresTimer() : ticksPerSecond(0)
{
	LARGE_INTEGER li;
	if (QueryPerformanceFrequency(&li)) {
		ticksPerSecond = LI2INT64(&li);
	}

	reset();
}

/////////////////////////////////////////////////////////////////////////////
HiresTimer::~HiresTimer()
{
}

/////////////////////////////////////////////////////////////////////////////
INT64 HiresTimer::LI2INT64(PLARGE_INTEGER li) const
{
	return (((INT64)(*li).HighPart) << 32) + (*li).LowPart;
}

/////////////////////////////////////////////////////////////////////////////
void HiresTimer::reset()
{
	QueryPerformanceCounter(&start);
}

/////////////////////////////////////////////////////////////////////////////
uint64_t HiresTimer::millis() const
{
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	uint64_t nmillis = 0;

	if (ticksPerSecond) {
		uint64_t tstart = LI2INT64((PLARGE_INTEGER)&start);
		uint64_t tend = LI2INT64(&end);
		nmillis = (tend - tstart) / (ticksPerSecond / 1000);	
	}

	return nmillis;
}

/////////////////////////////////////////////////////////////////////////////
HiresTimer::operator wstring() const
{
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	wchar_t buf[MAX_PATH] = { '\0' };

	if (ticksPerSecond) {
		uint64_t tstart = LI2INT64((PLARGE_INTEGER)&start);
		uint64_t tend = LI2INT64(&end);
		uint64_t ntime = (tend - tstart) / (ticksPerSecond / 100);

		int hours = (int)((ntime / 100) / 3600);
		int minutes = ((ntime / 100) % 3600) / 60;
		int seconds = (ntime / 100) % 60;
		int hundreths = ntime % 100;

		if (hours)
			_stprintf(buf, _T("%2d:%02d:%02d hours"), hours, minutes, seconds);

		else if (minutes)
			_stprintf(buf, _T("%2d:%02d minutes"), minutes, seconds);

		else
			_stprintf(buf, _T("%2d.%02d seconds"), seconds, hundreths);
	}

	return buf;
}