#pragma once
#pragma warning(disable:4996)	// disable deprecation warnings
#pragma warning(disable:4244)	// disable loss of data warning
#pragma warning(disable:4267)	// disable loss of data warning

#ifndef _UNICODE
#define _UNICODE
#define UNICODE
#endif // _UNICODE

#define NOMINMAX							// Don't use min/max
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <atlbase.h>
#include <stdint.h>
#include <limits.h>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <stack>

using std::wstring;
using std::wcout;
using std::wcerr;
using std::endl;
using std::wostream;
using std::wistream;
using std::wifstream;
using std::wstringstream;
using std::wistringstream;
using std::unique_ptr;

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
// ICallable interface
template <typename T>
DECLARE_INTERFACE(ICallable) {
public:
	virtual void Call(const T*, PVOID pExtra = NULL) PURE;
};

/////////////////////////////////////////////////////////////////////////////
// IWalkable interface
template <typename T>
DECLARE_INTERFACE(IWalkable) {
public:
	typedef ICallable<T> *PWALKER;
	virtual void Walk(PWALKER) PURE;
};
