#include "stdafx.h"
#include "btree.h"

Datum UTF8Datum(LPCTSTR value)
{
	Datum d;
	memset(&d, 0, sizeof(Datum));

#ifdef _UNICODE
	d.len = WideCharToMultiByte(CP_UTF8, 0, value, wcslen(value), 
		(LPSTR)d.buf, MAX_DATUM_LEN, NULL, NULL);
#else
	d.len = strlen(value);
	memcpy(d.buf, value, d.len);
#endif // _UNICODE

	return d;
}

int _tmain(int argc, _TCHAR* argv[])
{
	LPCTSTR filename = _T("c:\\temp\\test.idx");

	BTree tree;
	tree.open(filename, OM_CREATE);
	
	Datum key = UTF8Datum(_T("This is a key."));
	Datum value = UTF8Datum(_T("This is a value."));

	tree.insert(key, value);
	tree.close();

	tree.open(filename, OM_RO);
	Datum result = tree.search(key);
	tree.close();
	_tunlink(filename);

	ASSERT(result == value);

	return 0;
}

