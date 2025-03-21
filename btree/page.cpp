/////////////////////////////////////////////////////////////////////////////
//
// PAGE.CPP : On-disk page object
//

#include "stdafx.h"
#include "page.h"

/////////////////////////////////////////////////////////////////////////////
LPPAGE MakePage()
{
    return static_cast<LPPAGE>(VirtualAlloc(nullptr, PAGE_SIZE, MEM_COMMIT, PAGE_READWRITE));
}

/////////////////////////////////////////////////////////////////////////////
void FreePage(LPPAGE& page)
{
    VirtualFree(page, 0, MEM_RELEASE);
    VirtualFree(page, PAGE_SIZE, MEM_DECOMMIT);
    page = nullptr;
}
