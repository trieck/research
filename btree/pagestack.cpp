/////////////////////////////////////////////////////////////////////////////
//
// PAGESTACK.CPP : Stack of disk pages used during recursive search/insert
//

#include "stdafx.h"
#include "pagestack.h"

/////////////////////////////////////////////////////////////////////////////
PageStack::PageStack() : elements(INITIAL_SIZE), index(INITIAL_SIZE - 1)
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
    pages = static_cast<LPPAGE*>(malloc(sizeof(LPPAGE) * INITIAL_SIZE));

    for (uint32_t i = 0; i < INITIAL_SIZE; ++i) {
        pages[i] = MakePage();
    }
}

/////////////////////////////////////////////////////////////////////////////
void PageStack::reallocpages()
{
    index = elements;
    elements = elements * 2;
    pages = static_cast<LPPAGE*>(realloc(pages, sizeof(LPPAGE) * elements));

    for (uint32_t i = 0; i < index; ++i) {
        pages[index + i] = pages[i];
        pages[i] = MakePage();
    }
}

/////////////////////////////////////////////////////////////////////////////
void PageStack::freepages()
{
    for (uint32_t i = 0; i < elements; ++i) {
        FreePage(pages[i]);
    }

    free(pages);
}

/////////////////////////////////////////////////////////////////////////////
LPPAGE PageStack::push()
{
    if (index == 0)
        reallocpages(); // stack full

    return pages[index--];
}

/////////////////////////////////////////////////////////////////////////////
LPPAGE PageStack::pop()
{
    if (index >= elements - 1)
        return nullptr;

    return pages[index++];
}
