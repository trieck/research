/////////////////////////////////////////////////////////////////////////////
//
// PAGESTACK.CPP : Stack of disk pages used during recursive search/insert
//

#include "stdafx.h"
#include "pagestack.h"

/////////////////////////////////////////////////////////////////////////////
PageStack::PageStack() : elements(INITIAL_SIZE), index(INITIAL_SIZE-1)
{
	allocpages();
}

/////////////////////////////////////////////////////////////////////////////
PageStack::~PageStack()
{
	freepages();
}

/////////////////////////////////////////////////////////////////////////////
void PageStack::allocpages()
{
	pages = new LPPAGE[INITIAL_SIZE];

	for (uint32_t i = 0; i < INITIAL_SIZE; i++) {
		pages[i] = MakePage();
	}
}

/////////////////////////////////////////////////////////////////////////////
void PageStack::freepages()
{
	for (uint32_t i = 0; i < elements; i++) {
		FreePage(pages[i]);
	}

	delete [] pages;
}

/////////////////////////////////////////////////////////////////////////////
LPPAGE PageStack::push()
{
	if (index <= 0)
		return NULL;

	return pages[index--];
}

/////////////////////////////////////////////////////////////////////////////
LPPAGE PageStack::pop()
{
	if (index >= elements - 1)
		return NULL;

	return pages[index++];
}
