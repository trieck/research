#pragma once

#ifndef _UNICODE
#define _UNICODE
#define UNICODE
#endif // _UNICODE

#define WIN32_LEAN_AND_MEAN

#include "targetver.h"
#include <windows.h>
#include <atlbase.h>
#include <tchar.h>
#include <stdint.h>
#include <limits.h>
#include <string>
#include <iostream>

using std::wstring;
using std::wcout;
using std::wcerr;
using std::endl;
using std::wostream;

#ifndef ASSERT
#ifdef _DEBUG
#define ASSERT(f) if (!(f)) DebugBreak();
#define VERIFY(f) ASSERT(f)
#else
#define ASSERT(f) ((void)0)
#define VERIFY(f) ((void)(f))
#endif
#endif // ASSERT

/////////////////////////////////////////////////////////////////////////////
// file open modes
enum OpenMode { OM_CREATE, OM_TEMP, OM_RW, OM_RO };
