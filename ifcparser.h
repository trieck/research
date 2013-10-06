/////////////////////////////////////////////////////////////////////////////
//
// IFCPARSER.H : IFC STEP-21 file parser
//

#ifndef __IFCPARSER_H__
#define __IFCPARSER_H__

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
	wstring parseline(wstringvec& args);
	void parseargs(wistream& is, wstringvec& args);
	void comment();
	wstring getline();
	
	wifstream stream;
	uint64_t lineno;
};

#endif // __IFCPARSER_H__
