/////////////////////////////////////////////////////////////////////////////
//
// PAGE.H : On-disk page object
//

#ifndef __PAGE_H__
#define __PAGE_H__

#include "datum.h"

// size of a disk page
#define PAGE_SIZE			(4096)

typedef uint64_t pageid;

// ensure one byte alignment for structures
#pragma pack (push, 1)	

/////////////////////////////////////////////////////////////////////////////
typedef struct Node {
	pageid next;		// link to next subpage
	Datum key;			// key for node
	Datum value;		// value for leaf node
} *LPNODE;

/////////////////////////////////////////////////////////////////////////////
typedef struct PageHeader {
	uint8_t flags;	// page flags
	pageid pageno;	// page number
	uint16_t nodes;	// number of nodes on a page
} *LPPAGEHEADER;

/////////////////////////////////////////////////////////////////////////////
typedef struct Page {
	PageHeader header;	// page header
	Node data[1];				// page data
} *LPPAGE;

// restore default structure alignment
#pragma pack (pop)

// Helper functions
LPPAGE MakePage();
void FreePage(LPPAGE& page);

#endif // __PAGE_H__
