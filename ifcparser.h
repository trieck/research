/////////////////////////////////////////////////////////////////////////////
//
// IFCPARSER.H : IFC STEP-21 file parser
//

#ifndef __IFCPARSER_H__
#define __IFCPARSER_H__

#include "attribute.h"

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
	void parsedata();
	wstring parseentity(Attribute& attribs);
	void parseargs(wistream& is, Attribute& args);
	void parselist(wistream& is, Attribute& args);
	void comment();
	wstring getline();
	wstring gettok(wistream& is);
	wstring lookahead(wistream& is);

	wifstream stream;
	uint64_t lineno;
};

#endif // __IFCPARSER_H__
