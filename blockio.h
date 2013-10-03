/////////////////////////////////////////////////////////////////////////////
//
//	BLOCKIO.H : block i/o interface
//

#ifndef __BLOCKIO_H__
#define __BLOCKIO_H__

#include "RandStream.h"

/////////////////////////////////////////////////////////////////////////////
class blockio
{
// Construction / Destruction
public:
	blockio(uint32_t blocksize, uint32_t headersize = 0);
	virtual ~blockio();
	
// Interface
	bool open(LPCTSTR filename, OpenMode mode, uint32_t flags = 0);

	void close();
	
	uint32_t writeheader(const void *pv);
	uint32_t readblock(uint64_t blockno, void *pv);
	uint32_t writeblock(uint64_t blockno, const void *pv);
	uint32_t insertblock(const void *pv);
	uint32_t seekblock(uint64_t blockno);
	
	uint64_t tell();
	uint64_t getFileSize();

	bool isOpen() const { return stream != NULL; }

	uint16_t getBlocksize() const { return blocksize; }
	uint16_t getHeadersize() const { return headersize; }
	
	uint64_t nextblock() { return getFileSize() / blocksize; }

	bool setSize(uint64_t size);
	bool flush();

// Implementation
private:
	uint32_t read(void *pv, uint32_t len);
	uint32_t write(const void *pv, uint32_t len);

	uint32_t readEx(void *pv, uint32_t len, ULARGE_INTEGER offset);
	uint32_t writeEx(const void *pv, uint32_t len, ULARGE_INTEGER offset);

	uint32_t seek(int64_t offset, uint32_t whence);

	RandomFileStreamPtr stream;		// underlying stream
	uint32_t headersize;			// size of file header
	uint32_t blocksize;				// size of block
};
/////////////////////////////////////////////////////////////////////////////

#endif // __BLOCKIO_H__