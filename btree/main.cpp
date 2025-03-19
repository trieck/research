#include "stdafx.h"
#include "stree.h"
#include "HiresTimer.h"
#include <random>

namespace { // anonymous

wstring randomString()
{
    wstring output;
    wchar_t c;

    uint32_t len = MAX_DATUM_LEN / 4;

    output.resize(len);

    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);

    std::mt19937 eng(li.LowPart);

    for (uint32_t i = 0; i < len; ++i) {
        do {
            c = eng() % SCHAR_MAX;
        } while (!iswprint(c));
        output[i] = c;
    }

    return output;
}
} // anonymous

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 3) {
        wcerr << L"usage: btree filename count" << endl;
        return 1;
    }

    HiresTimer timer;

    StrTree tree;
    if (!tree.open(argv[1], OM_TEMP)) {
        wcerr << L"can't open file." << endl;
        return 2;
    }

    int ncount = _wtoi(argv[2]);

    for (int i = 0; i < ncount; i++) {
        wstring key = randomString();
        wstring value = randomString();

        wstring result = tree.search(key);
        if (!result.empty()) {
            i--; // duplicate
            continue;
        }

        tree.insert(key, value);

        result = tree.search(key);
        if (result != value) {
            wcerr << L"search mismatch!" << endl;
            return 3;
        }
    }

    tree.close();

    wcout << L"   elapsed time: " << timer << endl;

    return 0;
}
