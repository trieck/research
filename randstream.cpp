/////////////////////////////////////////////////////////////////////////////
//
//	RANDSTREAM.CPP : random access file based stream class
//	NOTE: NOT externally creatable
//
//	Copyright(c) 2006, KnowX.com, All Rights Reserved
//

#include "stdafx.h"
#include "RandStream.h"

/////////////////////////////////////////////////////////////////////////////
RandomFileStream::RandomFileStream()
	: hFile(INVALID_HANDLE_VALUE), hEvent(NULL), m_cRef(0)
{
	// The constructor AddRef's
	AddRef();
}

/////////////////////////////////////////////////////////////////////////////
RandomFileStream::~RandomFileStream()
{
	Close();
}

/////////////////////////////////////////////////////////////////////////////
RandomFileStream *RandomFileStream::Create(LPCTSTR lpFileName, 
	DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, 
	DWORD dwFlagsAndAttributes)
{
	RandomFileStream *pStream = new RandomFileStream();
	if (!pStream->Open(lpFileName, dwDesiredAccess, dwShareMode, 
		dwCreationDisposition, dwFlagsAndAttributes)) {
			pStream->Release();
			return NULL;		
	}

	return pStream;
}

/////////////////////////////////////////////////////////////////////////////
BOOL RandomFileStream::Open(LPCTSTR lpFileName, DWORD dwDesiredAccess,
	DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes)
{
	Close();
	hFile = CreateFile(lpFileName, dwDesiredAccess, dwShareMode, NULL,
		dwCreationDisposition, dwFlagsAndAttributes, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	return hEvent != NULL;
}

/////////////////////////////////////////////////////////////////////////////
void RandomFileStream::Close() 
{
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}

	if (hEvent != NULL) {
		CloseHandle(hEvent);
		hEvent = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
ULONG RandomFileStream::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

/////////////////////////////////////////////////////////////////////////////
ULONG RandomFileStream::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		delete this;
		return 0;
	}

	return m_cRef;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::QueryInterface(REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	if (riid == IID_IUnknown)
		*ppv = this;
	if (riid == IID_IStream || riid == IID_ISequentialStream)
		*ppv = this;

	if (*ppv) {
		((LPUNKNOWN)*ppv)->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	*pcbRead = 0;

	if (!pv)
		return STG_E_INVALIDPOINTER;

	if (cb == 0)
		return S_OK;

	if (!ReadFile(hFile, pv, cb, pcbRead, NULL))
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::ReadEx(void *pv, ULONG cb, ULARGE_INTEGER offset, 
	ULONG *pcbRead)
{
	*pcbRead = 0;

	if (!pv)
		return STG_E_INVALIDPOINTER;

	if (cb == 0)
		return S_OK;

	OVERLAPPED o;
	memset(&o, 0, sizeof(OVERLAPPED));
	o.Offset = offset.LowPart;
	o.OffsetHigh = offset.HighPart;
	o.hEvent = hEvent;

	if (!ResetEvent(hEvent))
		return HRESULT_FROM_WIN32(GetLastError());

	if (!ReadFileEx(hFile, pv, cb, &o, IOComplete))
		return HRESULT_FROM_WIN32(GetLastError());

	if (WaitForSingleObjectEx(hEvent, INFINITE, TRUE) != WAIT_IO_COMPLETION)
		return HRESULT_FROM_WIN32(GetLastError());

	if (!GetOverlappedResult(hFile, &o, pcbRead, FALSE))
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
	*pcbWritten = 0;

	if (!pv)
		return STG_E_INVALIDPOINTER;

	if (cb == 0)
		return S_OK;

	if (!WriteFile(hFile, pv, cb, pcbWritten, NULL))
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP RandomFileStream::WriteEx(const void *pv, ULONG cb, 
	ULARGE_INTEGER offset, ULONG *pcbWritten)
{
	*pcbWritten = 0;

	if (!pv)
		return STG_E_INVALIDPOINTER;

	if (cb == 0)
		return S_OK;

	OVERLAPPED o;
	memset(&o, 0, sizeof(OVERLAPPED));
	o.Offset = offset.LowPart;
	o.OffsetHigh = offset.HighPart;
	o.hEvent = hEvent;

	if (!ResetEvent(hEvent))
		return HRESULT_FROM_WIN32(GetLastError());

	if (!WriteFileEx(hFile, pv, cb, &o, IOComplete))
		return HRESULT_FROM_WIN32(GetLastError());

	if (WaitForSingleObjectEx(hEvent, INFINITE, TRUE) != WAIT_IO_COMPLETION)
		return HRESULT_FROM_WIN32(GetLastError());

	if (!GetOverlappedResult(hFile, &o, pcbWritten, FALSE))
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin,
	ULARGE_INTEGER *plibNewPosition)
{
	DWORD dwMoveMethod;

	switch(dwOrigin) {
	case STREAM_SEEK_SET:
		dwMoveMethod = FILE_BEGIN;
		break;
	case STREAM_SEEK_CUR:
		dwMoveMethod = FILE_CURRENT;
		break;
	case STREAM_SEEK_END:
		dwMoveMethod = FILE_END;
		break;
	default:   
		return STG_E_INVALIDFUNCTION;
		break;
	}

	DWORD dwPtrLow = SetFilePointer(hFile, dlibMove.LowPart, 
		&dlibMove.HighPart, dwMoveMethod);
	if (dwPtrLow == INVALID_SET_FILE_POINTER && 
		(GetLastError() != NO_ERROR)) {
			return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::SetSize(ULARGE_INTEGER libNewSize)
{
	LARGE_INTEGER size; 
	size.QuadPart = libNewSize.QuadPart;

	HRESULT hr = Seek(size, STREAM_SEEK_SET, NULL);
	if (FAILED(hr))
		return hr;

	if (!SetEndOfFile(hFile) && 
		(GetLastError() != NO_ERROR)) {
			return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::CopyTo(IStream *pstm, ULARGE_INTEGER cb,
	ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::Commit(DWORD grfCommitFlags)
{				
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::Revert()
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::LockRegion(ULARGE_INTEGER libOffset, 
	ULARGE_INTEGER cb, DWORD dwLockType)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::UnlockRegion(ULARGE_INTEGER libOffset,
	ULARGE_INTEGER cb, DWORD dwLockType)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
	pstatstg->cbSize.QuadPart = 0;

	DWORD dwSizeHigh;
	DWORD dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
	if (dwSizeLow == INVALID_FILE_SIZE && 
		(GetLastError() != NO_ERROR)) {
			return HRESULT_FROM_WIN32(GetLastError());
	}

	pstatstg->cbSize.LowPart = dwSizeLow;
	pstatstg->cbSize.HighPart = dwSizeHigh;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT RandomFileStream::Clone(IStream **ppstm)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP RandomFileStream::Tell(ULARGE_INTEGER *pPosition)
{
	if (pPosition == NULL)
		return E_POINTER;

	pPosition->QuadPart = 0;

	LONG high = 0;
	DWORD dwPtrLow = SetFilePointer(hFile, 0, 
		&high, FILE_CURRENT);
	if (dwPtrLow == INVALID_SET_FILE_POINTER && 
		(GetLastError() != NO_ERROR)) {
			return HRESULT_FROM_WIN32(GetLastError());
	}

	pPosition->LowPart = dwPtrLow; pPosition->HighPart = high;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP RandomFileStream::Flush()
{
	if (!FlushFileBuffers(hFile))
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP RandomFileStream::GetFileHandle(HANDLE *pHandle)
{
	*pHandle = hFile;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
VOID CALLBACK RandomFileStream::IOComplete(DWORD dwErrorCode,
	DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	SetEvent(lpOverlapped->hEvent);
}
