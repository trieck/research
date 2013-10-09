#pragma once
#pragma warning(disable:4996)	// disable deprecation warnings
#pragma warning(disable:4244)	// disable loss of data warning
#pragma warning(disable:4267)	// disable loss of data warning

#define NOMINMAX							// Don't use min/max
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <ObjBase.h>
#include <memory>
#include <string>
#include <iostream>
#include <random>

using std::string;
using std::ostream;
using std::cout;
using std::cerr;
using std::endl;

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