/////////////////////////////////////////////////////////////////////////////
//
//	BTREE.H : Disk based B-tree
//

#ifndef __BTREE_H__
#define __BTREE_H__

#include "blockio.h"
#include "datum.h"

typedef uint64_t pageid;

// ensure one byte alignment for structures below
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
	// Maximum depth of the b-tree.
	// The average number of probes p is about
	// logM(N).  So, we can have about M^p 
	// items in the tree before we exceed the depth 
	// limits of the tree.
	enum { MAXDEPTH = 10 };

	void allocpages();
	void freepages();
	bool readpage(pageid page_no, int level);
	bool writepage(LPPAGE h);
	bool insertpage(LPPAGE h);
	void clear();

	Datum searchR(LPPAGE h, const Datum& key, int level);
	LPPAGE insertR(LPPAGE h, const Node& node, int level);
	LPPAGE split(LPPAGE h);

	blockio io;								// block i/o
	OpenMode mode;						// read-write mode
	LPPAGE pages[MAXDEPTH];		// pages in memory
	LPPAGE frame[2];					// spare frames for page split
};

#endif // __BTREE_H__
