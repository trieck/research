/////////////////////////////////////////////////////////////////////////////
//
//	STREE.H : string based based B-tree
//

#ifndef __STREE_H__
#define __STREE_H__

#include "btree.h"

/////////////////////////////////////////////////////////////////////////////
class StrTree : BTree
{
    // Construction / Destruction
public:
    StrTree();
    ~StrTree() override;

    // Interface
    bool open(LPCTSTR filename, OpenMode m);
    void close();

    void insert(const wstring& key, const wstring& value);
    wstring search(const wstring& key);
};

#endif // __STREE_H__
