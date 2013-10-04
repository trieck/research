
#ifndef __DATUM_H__
#define __DATUM_H__

#define MAX_DATUM_LEN (80)

// ensure one byte alignment for structures
#pragma pack (push, 1)	

/////////////////////////////////////////////////////////////////////////////
typedef struct Datum {
	int8_t len;
	uint8_t buf[MAX_DATUM_LEN];
} *LPITEM;

// restore default structure alignment
#pragma pack (pop)

extern const Datum NullDatum;

// Datum helpers
Datum UTF8Datum(LPCWSTR value);
wstring DatumToString(const Datum& datum);
bool operator == (const Datum& lhs, const Datum& rhs);
bool operator < (const Datum& lhs, const Datum& rhs);
bool IsNullDatum(const Datum& d);

#endif // __DATUM_H__

