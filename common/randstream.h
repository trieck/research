/////////////////////////////////////////////////////////////////////////////
//
//	RANDSTREAM.H : random access file based stream class
//	NOTE: NOT externally creatable
//

#ifndef __RANDSTREAM_H__
#define __RANDSTREAM_H__

/////////////////////////////////////////////////////////////////////////////
class RandomFileStream : public IStream
{
// Construction / Destruction
private:
	RandomFileStream();
public:
	virtual ~RandomFileStream();

// Interface
	static RandomFileStream *Create(LPCTSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes);

	// inherited methods
	STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID *ppv);
    
    STDMETHODIMP Read(void *pv, ULONG cb,
            ULONG *pcbRead);

	STDMETHODIMP ReadEx(void *pv, ULONG cb, ULARGE_INTEGER offset,
		ULONG *pcbRead);

	STDMETHODIMP Write(const void *pv, ULONG cb,
            ULONG *pcbWritten);

	STDMETHODIMP WriteEx(const void *pv, ULONG cb, ULARGE_INTEGER offset,
            ULONG *pcbWritten);

	STDMETHODIMP Seek(LARGE_INTEGER dlibMove,
            DWORD dwOrigin,
            ULARGE_INTEGER *plibNewPosition);
        
	STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize);
        
	STDMETHODIMP CopyTo(IStream *pstm,
            ULARGE_INTEGER cb,
            ULARGE_INTEGER *pcbRead,
            ULARGE_INTEGER *pcbWritten);
        
	STDMETHODIMP Commit(DWORD grfCommitFlags);
        
	STDMETHODIMP Revert();
        
	STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset,
            ULARGE_INTEGER cb, DWORD dwLockType);
        
	STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset,
            ULARGE_INTEGER cb, DWORD dwLockType);
        
	STDMETHODIMP Stat(STATSTG *pstatstg, DWORD grfStatFlag);
        
	STDMETHODIMP Clone(IStream **ppstm);

	// New methods
	STDMETHODIMP Tell(ULARGE_INTEGER *pPosition);
	STDMETHODIMP Flush();
	STDMETHODIMP GetFileHandle(HANDLE *pHandle);

// Implementation
private:
	static VOID CALLBACK IOComplete(
		DWORD dwErrorCode,
		DWORD dwNumberOfBytesTransfered,
		LPOVERLAPPED lpOverlapped);

	BOOL Open(LPCTSTR lpFileName, DWORD dwDesiredAccess,
		DWORD dwShareMode, DWORD dwCreationDisposition, 
		DWORD dwFlagsAndAttributes);

	void Close();

	LONG m_cRef;	// reference count
	HANDLE hFile;	// file handle
	HANDLE hEvent;	// event handle for asynchronous i/o
};
/////////////////////////////////////////////////////////////////////////////

typedef CComPtr<RandomFileStream> RandomFileStreamPtr;

#endif // __RANDSTREAM_H__