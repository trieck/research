/////////////////////////////////////////////////////////////////////////////
//
// PAGE.H : On-disk page object
//

#ifndef __PAGE_H__
#define __PAGE_H__

#include "datum.h"

// size of a disk page
#define PAGE_SIZE			(4096)

using pageid = uint64_t;

// ensure one byte alignment for structures
#pragma pack (push, 1)

/////////////////////////////////////////////////////////////////////////////
using LPNODE = struct Node
{
    pageid next; // link to next subpage
    Datum key; // key for node
    Datum value; // value for leaf node
}*;

/////////////////////////////////////////////////////////////////////////////
using LPPAGEHEADER = struct PageHeader
{
    uint8_t flags; // page flags
    pageid pageno; // page number
    uint16_t nodes; // number of nodes on a page
}*;

/////////////////////////////////////////////////////////////////////////////
using LPPAGE = struct Page
{
    PageHeader header; // page header
    Node data[1]; // page data
}*;

// restore default structure alignment
#pragma pack (pop)

// Helper functions
LPPAGE MakePage();
void FreePage(LPPAGE& page);

#endif // __PAGE_H__
