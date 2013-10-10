/////////////////////////////////////////////////////////////////////////////
//
// ATTRIBUTE.CPP : Step file attribute.
//
// An attribute is defined recursively as a value or a list of other 
// attributes:
//
// attribute := value | list
// value := '{string}'
// list  := ( attribute, attribute, ... )
// 

#include "stdafx.h"
#include "attribute.h"

#define INITIAL_ALLOC_SIZE (10)

/////////////////////////////////////////////////////////////////////////////
Attribute::Attribute() : type(AT_EMPTY), value(NULL), numAttrs(0), 
	allocSize(0)
{
}

/////////////////////////////////////////////////////////////////////////////
Attribute::Attribute(const wchar_t *pval) : type(AT_VALUE), numAttrs(0), 
	allocSize(0)
{
	size_t len = wcslen(pval);
	value = new wchar_t[len + 1];
	wcscpy(value, pval);
}

/////////////////////////////////////////////////////////////////////////////
Attribute::~Attribute()
{
	clear();
}

/////////////////////////////////////////////////////////////////////////////
void Attribute::add(const wstring& value)
{
	add(new Attribute(value.c_str()));
}

/////////////////////////////////////////////////////////////////////////////
void Attribute::add(Attribute *pAttr)
{
	alloclist();
	attribs[numAttrs++] = pAttr;
}

/////////////////////////////////////////////////////////////////////////////
void Attribute::alloclist()
{
	if (type == AT_VALUE)
		clear();	// list morph

	type = AT_LIST;

	if (numAttrs < allocSize)
		return;	// space available

	if (allocSize == 0) {
		allocSize = INITIAL_ALLOC_SIZE;
		attribs = (Attribute**)malloc(allocSize * sizeof(Attribute*));
	} else {
		allocSize = allocSize * 2;
		attribs = (Attribute**)realloc(attribs, allocSize * sizeof(Attribute*));
	}
}

/////////////////////////////////////////////////////////////////////////////
void Attribute::clear()
{
	switch(type) {
	case AT_EMPTY:
	case AT_VALUE:
		delete [] value;
		value = NULL;
		break;
	case AT_LIST:
		clearR();
		break;
	}

	type = AT_EMPTY;
}

/////////////////////////////////////////////////////////////////////////////
void Attribute::clearR()
{
	Attribute *pAttr;
	for (uint32_t i = 0; i < numAttrs; ++i) {
		pAttr = attribs[i];
		delete pAttr;
	}

	free(attribs);
	attribs = NULL;
	numAttrs = allocSize = 0;
}