#include "stdafx.h"
#include "datum.h"

const Datum NullDatum = {SCHAR_MIN, '\0'};

/////////////////////////////////////////////////////////////////////////////
bool operator ==(const Datum& lhs, const Datum& rhs)
{
    if (lhs.len != rhs.len)
        return false;

    return memcmp(lhs.buf, rhs.buf, lhs.len) == 0;
}

/////////////////////////////////////////////////////////////////////////////
bool operator <(const Datum& lhs, const Datum& rhs)
{
    size_t len = min(lhs.len, rhs.len);
    return memcmp(lhs.buf, rhs.buf, len) < 0;
}

/////////////////////////////////////////////////////////////////////////////
bool IsNullDatum(const Datum& d)
{
    return d.len == SCHAR_MIN;
}

/////////////////////////////////////////////////////////////////////////////
Datum UTF8Datum(LPCWSTR value)
{
    Datum d;
    memset(&d, 0, sizeof(Datum));
    d.len = WideCharToMultiByte(CP_UTF8, 0, value, static_cast<int>(wcslen(value)),
                                (LPSTR)d.buf, MAX_DATUM_LEN, nullptr, nullptr);
    return d;
}

/////////////////////////////////////////////////////////////////////////////
wstring DatumToString(const Datum& datum)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)datum.buf, datum.len,
                                  nullptr, 0);

    auto pout = new wchar_t[len + 1];

    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)datum.buf, datum.len, pout, len);

    pout[len] = L'\0';

    wstring output = pout;

    delete [] pout;

    return output;
}
