#include "stdafx.h"
#include "btree.h"
#include "HiresTimer.h"

namespace {		// anonymous

Datum UTF8Datum(LPCTSTR value)
{
	Datum d;
	memset(&d, 0, sizeof(Datum));
	d.len = WideCharToMultiByte(CP_UTF8, 0, value, (int)wcslen(value), 
		(LPSTR)d.buf, MAX_DATUM_LEN, NULL, NULL);
	return d;
}

wstring DatumToString(const Datum& datum)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)datum.buf, datum.len, NULL, 0);

	wchar_t *pout = new wchar_t[len+1];

	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)datum.buf, datum.len, pout, len);

	pout[len] = L'\0';

	wstring output = pout;

	delete [] pout;

	return output;
}

wstring randomString(uint32_t len)
{
	wstring output;
	wchar_t c;

	output.resize(len);

	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);	

	srand(li.LowPart);

	for (uint32_t i = 0; i < len; ++i) {
		do {
			c = rand() % (USHRT_MAX+1);
		} while (!iswprint(c));
		output[i] = c;
	}
	
	return output;
}

Datum randomDatum()
{
	wstring r = randomString(MAX_DATUM_LEN/4);
	return UTF8Datum(r.c_str());
}

} // anonymous

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3) {
		wcerr << L"usage: btree filename count" << endl;
		exit(1);
	}

	HiresTimer timer;
	
	BTree tree;
	if (!tree.open(argv[1], OM_CREATE)) {
		wcerr << L"can't open file." << endl;
		exit(2);
	}
	
	int ncount = _ttoi(argv[2]);
	int dupes = 0;

	for (int i = 0; i < ncount; i++) {
		Datum key = randomDatum();
		Datum value = randomDatum();
		
		Datum result = tree.search(key);
		if (!IsNullDatum(result)) {
			dupes++;
			continue;
		}

		tree.insert(key, value);

		result = tree.search(key);
		if (!(result == value)) {
			wcerr << L"search mismatch!" << endl;
			wcerr << L"   Expected result : \"" << DatumToString(value) << L"\"" << endl;
			wcerr << L"   Found result    : \"" << DatumToString(result) << L"\"" << endl;
			exit(1);
		}
	}

	tree.close();

	_tunlink(argv[1]);

	wcout << L"   elapsed time: " << timer << endl;
	wcout << L" dupe key count: " << dupes << endl;

	return 0;
}

