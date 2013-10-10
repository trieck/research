/////////////////////////////////////////////////////////////////////////////
//
//	BTREE.H : Disk based B-tree
//

#ifndef __BTREE_H__
#define __BTREE_H__

#include "blockio.h"
#include "pagestack.h"

/////////////////////////////////////////////////////////////////////////////
class BTree
{
	// Construction / Destruction
public:
	BTree();
	virtual ~BTree();

	// Interface
public:
	bool open(LPCTSTR filename, OpenMode m);
	void close();

	void insert(const Datum& key, const Datum& value);
	Datum search(const Datum& key);

	// Implementation
private:
	bool readpage(pageid page_no, LPPAGE h);
	bool writepage(LPPAGE h);
	bool insertpage(LPPAGE h);
	void clear();
	void pushpage();
	void poppage();

	Datum searchR(LPPAGE h, const Datum& key);
	Datum searchN(pageid page_no, const Datum& key);
	LPPAGE insertR(LPPAGE h, const Node& node);
	LPPAGE insertN(pageid page_no, const Node& node);
	LPPAGE split(LPPAGE h);

	blockio io;								// block i/o
	OpenMode mode;						// read-write mode	
	LPPAGE frame[2];					// spare frames for page split
	LPPAGE root;							// root page
	LPPAGE current;						// current page
	PageStack pagestack;			// stack of page pointers
};

#endif // __BTREE_H__
