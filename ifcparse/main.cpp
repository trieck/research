#include "stdafx.h"
#include "ifcparser.h"
#include "exception.h"
#include "HiresTimer.h"

int wmain(int argc, wchar_t* argv[])
{
	if (argc < 2) {
		wcerr << L"usage: ifcparse filename" << endl;
		return 1;
	}
		
	HiresTimer timer;
	IFCParser parser;

	try {
		parser.parse(argv[1]);
	} catch (const Exception& e) {
		wcerr << e.getDescription() << endl;
		return 2;
	}

	wcout << L"   elapsed time: " << timer << endl;

	return 0;
}

