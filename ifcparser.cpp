/////////////////////////////////////////////////////////////////////////////
//
// IFCPARSER.CPP : IFC STEP-21 file parser
//

#include "stdafx.h"
#include "ifcparser.h"
#include "exception.h"
#include "field.h"

#define FILE_TYPE						L"ISO-10303-21"
#define HEADER							L"HEADER"
#define FILE_DESCRIPTION		L"FILE_DESCRIPTION"
#define FILE_NAME						L"FILE_NAME"
#define FILE_SCHEMA					L"FILE_SCHEMA"
#define END_SECTION					L"ENDSEC"

#define THROW_BAD_FORMAT(s,n)	\
	throw Exception(L"bad file format: expected \"%s\" at line#%I64u.", s, n)

/////////////////////////////////////////////////////////////////////////////
IFCParser::IFCParser() : lineno(1)
{
}

/////////////////////////////////////////////////////////////////////////////
IFCParser::~IFCParser()
{
	stream.close();
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::parse(LPCWSTR filename)
{
	stream.open(filename);
	if (!stream.is_open())
		throw Exception(L"Unable to open file \"%s\".", filename);

	parse();
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::parse()
{
	wstring line = getline();
	if (line != FILE_TYPE)
		throw Exception(L"bad file format: expected \"%s\" at line#%I64u.", 
			FILE_TYPE, lineno);

	parseheader();
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::parseheader()
{
	// header
	wstring line = getline();
	if (line != HEADER)
		THROW_BAD_FORMAT(HEADER, lineno);

	// file description
	wstringvec args;
	line = parseline(args);
	if (line != FILE_DESCRIPTION)
		THROW_BAD_FORMAT(FILE_DESCRIPTION, lineno);

	// filename
	line = parseline(args);
	if (line != FILE_NAME)
		THROW_BAD_FORMAT(FILE_NAME, lineno);

	// file schema
	line = parseline(args);
	if (line != FILE_SCHEMA)
		THROW_BAD_FORMAT(FILE_SCHEMA, lineno);

	// end header
	line = getline();
	if (line != END_SECTION)
		THROW_BAD_FORMAT(END_SECTION,lineno);
}

/////////////////////////////////////////////////////////////////////////////
wstring IFCParser::getline()
{
	wstringstream ss;

	int tickm = 0;
	wchar_t c;

	while ((c = stream.get()) != WEOF) {
		switch (c) {
		case '\r':
		case '\n':
			lineno++;
			continue;
		case '\'':
			ss << c;
			tickm = !tickm;
			break;
		case '/':
			if (stream.peek() == '*')
				comment();
			else
				ss << c;
			break;
		case ';':
			if (!tickm) return ss.str();	// EOL
			ss << c;
			break;
		default:
			ss << c;
			break;
		}
	}
	
	return L"";
}

/////////////////////////////////////////////////////////////////////////////
wstring IFCParser::parseline(wstringvec& args)
{
	args.clear();

	wstring line = getline();
	if (line.length() == 0)
		return L"";

	wistringstream ss(line);
	wstringstream entity;

	wchar_t c;
	while ((c = ss.get()) != WEOF) {
		switch (c) {
		case '(':	// args
			parseargs(ss, args);
			break;
		default:
			entity << c;
			break;
		}
	}

	return entity.str();
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::parseargs(wistream& is, wstringvec& args)
{
	int level = 1;

	wchar_t c;
	while ((c = is.get()) != WEOF) {
		switch (c) {
		case '(':
			// parselist
			++level;
			break;
		case ')':
			--level;
			break;
		case ',':
			break;
		case '\\':
			break;
		default:
			break;
		}
	}

	if (level != 0)
		throw Exception(L"Unable to parse fields at line#%I64u.", lineno);
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::comment()
{
	stream.get();	// '*'

	wchar_t c;
	while ((c = stream.get()) != WEOF) {
		switch (c) {
		case '*':
			if (stream.peek() != '/')
				throw Exception(L"Malformed comment at line#%I64u.", lineno);
			stream.get();	// '/'
			return;
		default:
			break;
		}
	}

	throw Exception(L"Malformed comment at line#%I64u.", lineno);
}
