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
    RandomFileStream();

public:
    virtual ~RandomFileStream();

    // Interface
    static RandomFileStream* Create(LPCTSTR lpFileName,
                                    DWORD dwDesiredAccess,
                                    DWORD dwShareMode,
                                    DWORD dwCreationDisposition,
                                    DWORD dwFlagsAndAttributes);

    // inherited methods
    STDMETHODIMP_(ULONG) AddRef(void) override;
    STDMETHODIMP_(ULONG) Release(void) override;
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv) override;

    STDMETHODIMP Read(void* pv, ULONG cb,
                      ULONG* pcbRead) override;

    STDMETHODIMP ReadEx(void* pv, ULONG cb, ULARGE_INTEGER offset,
                        ULONG* pcbRead);

    STDMETHODIMP Write(const void* pv, ULONG cb,
                       ULONG* pcbWritten) override;

    STDMETHODIMP WriteEx(const void* pv, ULONG cb, ULARGE_INTEGER offset,
                         ULONG* pcbWritten);

    STDMETHODIMP Seek(LARGE_INTEGER dlibMove,
                      DWORD dwOrigin,
                      ULARGE_INTEGER* plibNewPosition) override;

    STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize) override;

    STDMETHODIMP CopyTo(IStream* pstm,
                        ULARGE_INTEGER cb,
                        ULARGE_INTEGER* pcbRead,
                        ULARGE_INTEGER* pcbWritten) override;

    STDMETHODIMP Commit(DWORD grfCommitFlags) override;

    STDMETHODIMP Revert() override;

    STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset,
                            ULARGE_INTEGER cb, DWORD dwLockType) override;

    STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset,
                              ULARGE_INTEGER cb, DWORD dwLockType) override;

    STDMETHODIMP Stat(STATSTG* pstatstg, DWORD grfStatFlag) override;

    STDMETHODIMP Clone(IStream** ppstm) override;

    // New methods
    STDMETHODIMP Tell(ULARGE_INTEGER* pPosition);
    STDMETHODIMP Flush();
    STDMETHODIMP GetFileHandle(HANDLE* pHandle);

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

    LONG m_cRef; // reference count
    HANDLE hFile; // file handle
    HANDLE hEvent; // event handle for asynchronous i/o
};

/////////////////////////////////////////////////////////////////////////////

using RandomFileStreamPtr = CComPtr<RandomFileStream>;

#endif // __RANDSTREAM_H__
