/////////////////////////////////////////////////////////////////////////////
//
//	BLOCKIO.CPP : block i/o interface
//

#include "common.h"
#include "blockio.h"

/////////////////////////////////////////////////////////////////////////////
blockio::blockio(uint32_t bsize, uint32_t hsize)
    : headersize(hsize), blocksize(bsize)
{
}

/////////////////////////////////////////////////////////////////////////////
blockio::~blockio()
{
    close();
}

/////////////////////////////////////////////////////////////////////////////
bool blockio::open(LPCTSTR filename, OpenMode mode, uint32_t flags)
{
    close();

    uint32_t access = 0, disp = 0;
    uint32_t attr = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS |
        FILE_FLAG_OVERLAPPED | flags;
    uint32_t share = FILE_SHARE_READ | FILE_SHARE_WRITE;

    switch (mode) {
    case OM_CREATE:
        access |= GENERIC_READ | GENERIC_WRITE;
        disp |= CREATE_ALWAYS;
        break;
    case OM_TEMP:
        access |= GENERIC_READ | GENERIC_WRITE;
        disp |= CREATE_ALWAYS;
        attr |= FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE;
        share |= FILE_SHARE_DELETE;
        break;
    case OM_RW:
        access |= GENERIC_READ | GENERIC_WRITE;
        disp |= OPEN_ALWAYS;
        break;
    case OM_RO:
    default:
        access |= GENERIC_READ;
        disp |= OPEN_EXISTING;
        break;
    }

    stream.Attach(RandomFileStream::Create(filename, access, share, disp,
                                           attr));
    if (stream == nullptr)
        return false;

    return true;
}

/////////////////////////////////////////////////////////////////////////////
void blockio::close()
{
    if (stream != nullptr) {
        stream.Release();
        stream = nullptr;
    }
}

/////////////////////////////////////////////////////////////////////////////
bool blockio::setSize(uint64_t size)
{
    ULARGE_INTEGER sz;
    sz.QuadPart = size;
    return SUCCEEDED(stream->SetSize(sz));
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::read(void* pv, uint32_t len)
{
    ULARGE_INTEGER li;
    li.QuadPart = 0;
    return readEx(pv, len, li);
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::readEx(void* pv, uint32_t len, ULARGE_INTEGER offset)
{
    ULONG nread;
    stream->ReadEx(pv, len, offset, &nread);
    return nread;
}

/////////////////////////////////////////////////////////////////////////////
bool blockio::flush()
{
    return SUCCEEDED(stream->Flush());
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::readblock(uint64_t blockno, void* pv)
{
    ULARGE_INTEGER li;
    li.QuadPart = blockno * blocksize + headersize;
    return readEx(pv, blocksize, li);
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::write(const void* pv, uint32_t len)
{
    ULARGE_INTEGER li;
    li.QuadPart = 0;
    return writeEx(pv, len, li);
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::writeEx(const void* pv, uint32_t len, ULARGE_INTEGER offset)
{
    ULONG nwritten;
    stream->WriteEx(pv, len, offset, &nwritten);
    return nwritten;
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::writeheader(const void* pv)
{
    if (!seek(0, STREAM_SEEK_SET))
        return 0;

    return write(pv, headersize);
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::writeblock(uint64_t blockno, const void* pv)
{
    ULARGE_INTEGER li;
    li.QuadPart = blockno * blocksize + headersize;
    return writeEx(pv, blocksize, li);
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::insertblock(const void* pv)
{
    ULARGE_INTEGER li;
    li.LowPart = li.HighPart = 0xFFFFFFFF; // EOF
    return writeEx(pv, blocksize, li);
}

/////////////////////////////////////////////////////////////////////////////
uint64_t blockio::tell()
{
    ULARGE_INTEGER pos;
    HRESULT hr = stream->Tell(&pos);
    if (FAILED(hr))
        return 0;

    return pos.QuadPart;
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::seekblock(uint64_t blockno)
{
    int64_t offset = blockno * blocksize + headersize;
    return seek(offset, STREAM_SEEK_SET);
}

/////////////////////////////////////////////////////////////////////////////
uint32_t blockio::seek(int64_t offset, uint32_t whence)
{
    LARGE_INTEGER li;
    li.QuadPart = offset;
    HRESULT hr = stream->Seek(li, whence, nullptr);
    return SUCCEEDED(hr);
}

/////////////////////////////////////////////////////////////////////////////
uint64_t blockio::getFileSize()
{
    STATSTG stg;
    HRESULT hr = stream->Stat(&stg, STATFLAG_NONAME);
    if (FAILED(hr))
        return 0;

    return stg.cbSize.QuadPart;
}
