/////////////////////////////////////////////////////////////////////////////
//
// IFCPARSER.H : IFC STEP-21 file parser
//

#ifndef __IFCPARSER_H__
#define __IFCPARSER_H__

#include "field.h"

class IFCParser
{
	// Construction / Destruction
public:
	IFCParser();
	~IFCParser();

	// Interface
	void parse(LPCWSTR filename);

	// Implementation
private:
	void parse();
	void parseheader();
	wstring parseline(fieldvec& args);
	void parseargs(wistream& is, fieldvec& args);
	void comment();
	wstring getline();
	wstring gettok(wistream& is);

	wifstream stream;
	uint64_t lineno;
};

#endif // __IFCPARSER_H__
