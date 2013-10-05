/////////////////////////////////////////////////////////////////////////////
//
// PAGESTACK.H : Stack of disk pages used during recursive search/insert
//

#ifndef __PAGESTACK_H__
#define __PAGESTACK_H__

#include "page.h"

/////////////////////////////////////////////////////////////////////////////
class PageStack
{
	// Construction / Destruction
public:
	PageStack();
	~PageStack();

	// Interface
	LPPAGE push();
	LPPAGE pop();

	// Implementation
private:
	void allocpages();
	void freepages();
	enum { INITIAL_SIZE = 10 };

	LPPAGE* pages;			// array of page pointers
	uint32_t elements;	// number of elements in array
	uint32_t index;			// stack pointer
};

#endif // __PAGESTACK_H__


