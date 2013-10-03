// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <atlbase.h>
#include <tchar.h>
#include <stdint.h>
#include <limits.h>

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
