/////////////////////////////////////////////////////////////////////////////
//
// IFCPARSER.CPP : IFC STEP-21 file parser
//

#include "stdafx.h"
#include "ifcparser.h"
#include "exception.h"

#define FILE_TYPE						L"ISO-10303-21"
#define HEADER							L"HEADER"
#define FILE_DESCRIPTION		L"FILE_DESCRIPTION"
#define FILE_NAME						L"FILE_NAME"
#define FILE_SCHEMA					L"FILE_SCHEMA"
#define END_SECTION					L"ENDSEC"
#define DATA								L"DATA"
#define END_FILE_TYPE				L"END-ISO-10303-21"

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
		THROW_BAD_FORMAT(FILE_TYPE, lineno);

	parseheader();
	parsedata();

	line = getline();
	if (line != END_FILE_TYPE)
		THROW_BAD_FORMAT(END_FILE_TYPE, lineno);
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::parseheader()
{
	// header
	wstring line = getline();
	if (line != HEADER)
		THROW_BAD_FORMAT(HEADER, lineno);

	// file description
	Attribute attribs;
	wstring entity = parseentity(attribs);
	if (entity != FILE_DESCRIPTION)
		THROW_BAD_FORMAT(FILE_DESCRIPTION, lineno);

	// filename
	entity = parseentity(attribs);
	if (entity != FILE_NAME)
		THROW_BAD_FORMAT(FILE_NAME, lineno);

	// file schema
	entity = parseentity(attribs);
	if (entity != FILE_SCHEMA)
		THROW_BAD_FORMAT(FILE_SCHEMA, lineno);

	// end header
	line = getline();
	if (line != END_SECTION)
		THROW_BAD_FORMAT(END_SECTION,lineno);
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::parsedata()
{
	// data
	wstring line = getline();
	if (line != DATA)
		THROW_BAD_FORMAT(DATA, lineno);

	Attribute attribs;
	wstring entity;

	do {
		entity = parseentity(attribs);
	} while (entity.length() > 0 && entity[0] == '#');

	if (entity != END_SECTION)
		THROW_BAD_FORMAT(END_SECTION, lineno);
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
wstring IFCParser::parseentity(Attribute& args)
{
	args.clear();

	wstring line = getline();
	if (line.length() == 0)
		return L"";

	wistringstream ss(line);
	
	wstring entity = gettok(ss);
	parseargs(ss, args);

	return entity;
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::parseargs(wistream& is, Attribute& args)
{
	args.clear();

	wstring tok = gettok(is);	// '('

	parselist(is, args);

	tok = gettok(is);					// ')'	
}

/////////////////////////////////////////////////////////////////////////////
void IFCParser::parselist(wistream& is, Attribute& args)
{
	args.clear();

	wstring tok;
	while ((tok = lookahead(is)).length() > 0) {
		if (tok[0] == L',') {
			gettok(is);
			continue;	// more
		}

		if (tok[0] == L')')
			break;

		if (tok[0] == L'(') {
			Attribute* f = new Attribute();	// add a list
			parseargs(is, *f);
			args.add(f);
			continue;
		}

		tok = gettok(is);

		args.add(tok);	// add a value
	}
}

/////////////////////////////////////////////////////////////////////////////
wstring IFCParser::lookahead(wistream& is)
{
	wistream::pos_type pos = is.tellg();

	wstring tok = gettok(is);

	is.seekg(pos);

	return tok;
}

/////////////////////////////////////////////////////////////////////////////
wstring IFCParser::gettok(wistream& is)
{
	wstringstream token;

	wchar_t c;
	while ((c = is.get()) != WEOF) {
		switch (c) {
		case '(':
		case ')':
		case ',':
		if (token.str().length() > 0) {
				is.unget();
				return token.str();
			}
			token << c;
			return token.str();
		case '\'':
			if (token.str().length() > 0) {
				is.unget();
				return token.str();
			}
			while ((c = is.get()) != EOF) {
				if (c == '\'')
					break;
				token << c;
			}
			return token.str();
		default:
			token << c;
			break;
		}
	}

	return token.str();
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
