/////////////////////////////////////////////////////////////////////////////
//
//	EXCEPTION.CPP : simple exception class
//

#include "common.h"
#include "exception.h"

/////////////////////////////////////////////////////////////////////////////
Exception::Exception(const wchar_t* fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

    wchar_t buff[1000];
    _vsnwprintf(buff, 1000, fmt, arglist);

    va_end(arglist);

    description = buff;
}
