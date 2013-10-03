/////////////////////////////////////////////////////////////////////////////
//
//	BTREE.CPP : Disk based B-tree
//

#include "stdafx.h"
#include "btree.h"

// size of a disk page
#define PAGE_SIZE			(4096)

// page type flags
#define PTF_LEAF			(1 << 0)

// helper macros
#define ISLEAF(p)			(p->header.flags & PTF_LEAF)
#define SETLEAF(p)		(p->header.flags |= PTF_LEAF)
#define SETINTERNAL(p)(p->header.flags &= ~PTF_LEAF)
#define PAGENO(p)			(p->header.pageno)
#define NODES(p)			(p->header.nodes)

// maximum number of nodes on a page, must be even to split a page
#define MAXNODES	\
	((PAGE_SIZE - sizeof(PageHeader)) / sizeof(Node) & ~1)

// nodes accessors
#define NODE(p, n)		(p->data[n])
#define KEY(p, n)			(p->data[n].key)
#define VALUE(p, n)		(p->data[n].value)
#define NEXT(p, n)		(p->data[n].next)

// allocate a new page
#define MKPAGE()		\
	((LPPAGE)VirtualAlloc(NULL, PAGE_SIZE, MEM_COMMIT, PAGE_READWRITE))

// free an allocated page
#define FREEPAGE(p)														\
	do {																				\
		VirtualFree(p, 0, MEM_RELEASE);						\
		VirtualFree(p, PAGE_SIZE, MEM_DECOMMIT);	\
		p = NULL;																	\
	} while(0)

namespace { const Datum NullDatum = { SCHAR_MIN, '\0' }; }

/////////////////////////////////////////////////////////////////////////////
bool operator == (const Datum& lhs, const Datum& rhs) 
{
	if (lhs.len != rhs.len)
		return false;

	return memcmp(lhs.buf, rhs.buf, lhs.len) == 0;
}

/////////////////////////////////////////////////////////////////////////////
bool operator < (const Datum& lhs, const Datum& rhs) 
{
	size_t len = min(lhs.len, rhs.len);
	return memcmp(lhs.buf, rhs.buf, len) < 0;
}

/////////////////////////////////////////////////////////////////////////////
bool isNull(const Datum& d)
{
	return d == NullDatum;
}

/////////////////////////////////////////////////////////////////////////////
BTree::BTree()
	: io(PAGE_SIZE)
{
	allocpages();
}

/////////////////////////////////////////////////////////////////////////////
BTree::~BTree()
{
	close();
	freepages();
}

/////////////////////////////////////////////////////////////////////////////
void BTree::allocpages()
{
	for (int i = 0; i < MAXDEPTH; i++) {
		pages[i] = MKPAGE();
	}
	frame[0] = MKPAGE();
	frame[1] = MKPAGE();
}

/////////////////////////////////////////////////////////////////////////////
void BTree::freepages()
{
	for (int i = 0; i < MAXDEPTH; i++) {
		FREEPAGE(pages[i]);
	}
	FREEPAGE(frame[0]);
	FREEPAGE(frame[1]);
}

/////////////////////////////////////////////////////////////////////////////
void BTree::close()
{
	io.close();
}

/////////////////////////////////////////////////////////////////////////////
void BTree::clear()
{
	for (int i = 0; i < MAXDEPTH; i++) {
		memset(pages[i], 0, PAGE_SIZE);
	}
	memset(frame[0], 0, PAGE_SIZE);
	memset(frame[1], 0, PAGE_SIZE);
}

/////////////////////////////////////////////////////////////////////////////
bool BTree::open(LPCTSTR filename, OpenMode m)
{
	clear();
	if (!io.open(filename, mode = m))
		return false;

	uint64_t size = io.getFileSize();
	if (size != 0)
		return readpage(0, 0);			// read page zero

	SETLEAF(pages[0]);						// set root page to leaf
	return writepage(pages[0]);		// write page zero
}

/////////////////////////////////////////////////////////////////////////////
bool BTree::readpage(pageid page_no, int level)
{
	return io.readblock(page_no, pages[level]) == PAGE_SIZE;
}

/////////////////////////////////////////////////////////////////////////////
bool BTree::writepage(LPPAGE h)
{
	return io.writeblock(PAGENO(h), h) == PAGE_SIZE;
}

/////////////////////////////////////////////////////////////////////////////
bool BTree::insertpage(LPPAGE h)
{
	PAGENO(h) = io.getFileSize() / PAGE_SIZE;
	return io.insertblock(h) == PAGE_SIZE;
}

/////////////////////////////////////////////////////////////////////////////
Datum BTree::search(const Datum& key)
{
	return searchR(pages[0], key, 0);
}

/////////////////////////////////////////////////////////////////////////////
Datum BTree::searchR(LPPAGE h, const Datum& key, int level)
{
	int j;

	if (ISLEAF(h)) { // leaf page
		for (j = 0; j < NODES(h); j++) {
			if (KEY(h, j) == key)
				return VALUE(h, j);
		}
	} else { // internal page
		for (j = 0; j < NODES(h); j++) {
			if ((j+1 == NODES(h) || key < KEY(h, j+1))) {
				if (!readpage(NEXT(h, j), level+1))
					return NullDatum;
				return searchR(pages[level+1], key, level+1);
			}
		}
	}

	return NullDatum;
}

/////////////////////////////////////////////////////////////////////////////
void BTree::insert(const Datum& key, const Datum& value)
{
	Node node;
	node.next = 0;
	node.key = key;
	node.value = value;

	LPPAGE u = insertR(pages[0], node, 0);
	if (u == 0) return;

	// The basic idea with the root page split is that we create a new 
	// internal root page t with 2 links. The first link points to 
	// the old root page and the second link points to the page that caused 
	// the split.  The height of the tree is increased by one.
	LPPAGE t = frame[1];
	memset(t, 0, PAGE_SIZE);

	insertpage(pages[0]);	// relocate old root page

	SETINTERNAL(t);
	NODES(t) = 2;
	KEY(t, 0) = KEY(pages[0], 0);
	KEY(t, 1) = KEY(u, 0);
	NEXT(t, 0) = PAGENO(pages[0]); NEXT(t, 1) = PAGENO(u);

	memcpy(pages[0], t, PAGE_SIZE);
	writepage(pages[0]);
}

/////////////////////////////////////////////////////////////////////////////
LPPAGE BTree::insertR(LPPAGE h, const Node& node, int level)
{
	int i, j;
	Node t = node;
	
	if (ISLEAF(h)) { // leaf page
		for (j = 0; j < NODES(h); j++) {
			if (node.key < KEY(h, j)) break;
		}
	} else { // internal page
		for (j = 0; j < NODES(h); j++) {
			if ((j+1 == NODES(h) || node.key < KEY(h, j+1))) {
				LPPAGE u; 
				if (!readpage(NEXT(h, j++), level+1))
					return 0;
				u = insertR(pages[level+1], node, level+1);
				if (u == 0) return 0;
				t.key = KEY(u, 0); t.next = PAGENO(u);
				break;
			}
		}
	}

	for (i = NODES(h); i > j; i--) NODE(h, i) = NODE(h, i-1);
	NODE(h, j) = t;
	if (++NODES(h) < MAXNODES) {
		writepage(h);		// write dirty page
		return 0; 
	} 

	return split(h);	
}

/////////////////////////////////////////////////////////////////////////////
LPPAGE BTree::split(LPPAGE h)
{
	LPPAGE t = frame[0];
	memset(t, 0, PAGE_SIZE);

	t->header.flags = h->header.flags;

	for (int j = 0; j < MAXNODES/2; j++) 
		NODE(t, j) = NODE(h, MAXNODES/2+j);

	NODES(h) = MAXNODES/2; NODES(t) = MAXNODES/2;

	writepage(h);
	insertpage(t);

	return t;
}
