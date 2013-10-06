#include "stdafx.h"
#include "ifcparser.h"
#include "exception.h"

int wmain(int argc, wchar_t* argv[])
{
	if (argc < 2) {
		wcerr << L"usage: ifcparse filename" << endl;
		return 1;
	}
		
	IFCParser parser;

	try {
		parser.parse(argv[1]);
	} catch (const Exception& e) {
		wcerr << e.getDescription() << endl;
		return 2;
	}

	return 0;
}

