/////////////////////////////////////////////////////////////////////////////
//
//	STREE.H : string based based B-tree
//

#ifndef __STREE_H__
#define __STREE_H__

#include "btree.h"

/////////////////////////////////////////////////////////////////////////////
class StrTree : private BTree
{
	// Construction / Destruction
public:
	StrTree();
	virtual ~StrTree();

	// Interface
public:
	bool open(LPCTSTR filename, OpenMode m);
	void close();

	void insert(const wstring& key, const wstring& value);
	wstring search(const wstring& key);
};

#endif // __STREE_H__
