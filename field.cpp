/////////////////////////////////////////////////////////////////////////////
//
// FIELD.H : Step file field.
//
// A field is defined recursively as a string or a list of other fields.
//

#include "stdafx.h"
#include "field.h"

/////////////////////////////////////////////////////////////////////////////
Field::Field() : type(FT_EMPTY), value(NULL), numFields(0)
{
}

/////////////////////////////////////////////////////////////////////////////
Field::Field(const wchar_t *pval) : type(FT_VALUE), numFields(0)
{
	size_t len = wcslen(pval);
	value = new wchar_t[len + 1];
	wcscpy(value, pval);
}

/////////////////////////////////////////////////////////////////////////////
Field::~Field()
{
	destroy();
}

/////////////////////////////////////////////////////////////////////////////
void Field::destroy()
{
	switch(type) {
	case FT_EMPTY:
	case FT_VALUE:
		delete [] value;
		value = NULL;
		break;
	case FT_LIST:
		destroyV();
		break;
	}

	type = FT_EMPTY;
}

/////////////////////////////////////////////////////////////////////////////
void Field::destroyV()
{
	ASSERT(type == FT_LIST);

	Field *pfield;
	for (uint32_t i = 0; i < numFields; ++i) {
		pfield = *fields++;
		pfield->destroy();
	}

	delete [] fields;
	fields = NULL;
	numFields = 0;
}