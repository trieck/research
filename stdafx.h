#pragma once
#pragma warning(disable:4996)	// disable deprecation warnings
#pragma warning(disable:4244)	// disable loss of data warning
#pragma warning(disable:4267)	// disable loss of data warning

#define NOMINMAX							// Don't use min/max
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <memory>
#include <string>
#include <iostream>
#include <random>

using std::string;
using std::ostream;
using std::cout;
using std::cerr;
using std::endl;

