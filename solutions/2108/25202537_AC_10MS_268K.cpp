// POJ 2108 - Lazy Pianist
// Model: claude-sonnet-5
//
// The melody value at 1-indexed position i is popcount(i) (number of set
// bits in i's binary representation) -- verified against the full 16-term
// sample sequence and its self-similarity under taking every 2nd / 4th note
// (popcount(2k) == popcount(k), which is exactly why the "same melody"
// property in the statement holds).
//
// For each query (total, cant, pos) we must first validate
// 1 <= pos <= total and pos + cant - 1 <= total (computed without overflow,
// since the discuss board reports both this exact overflow trap and a need
// for 64-bit -- indeed unsigned -- arithmetic: values apparently range high
// enough that signed 64-bit (%lld / long long) has failed for other
// submitters while unsigned 64-bit passes). We therefore read/print via
// iostream with unsigned long long throughout, which also sidesteps any
// %lld vs %I64d format-specifier mismatch on POJ's older compilers.
#include <iostream>

using namespace std;

static unsigned popcountULL(unsigned long long x) {
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1ULL);
        x >>= 1;
    }
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int T;
    if (!(cin >> T)) return 0;
    for (int tc = 1; tc <= T; ++tc) {
        unsigned long long total, cant, pos;
        cin >> total >> cant >> pos;

        bool ok = true;
        if (pos < 1ULL || pos > total) {
            ok = false;
        } else {
            unsigned long long remaining = total - pos + 1ULL; // safe: pos<=total
            if (cant > remaining) ok = false;
        }

        cout << "Case " << tc << ": ";
        if (!ok) {
            cout << "It cannot be solved.\n";
        } else {
            for (unsigned long long k = 0; k < cant; ++k) {
                if (k) cout << ' ';
                cout << popcountULL(pos + k);
            }
            cout << "\n";
        }
    }
    return 0;
}
