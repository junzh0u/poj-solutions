// POJ 3307 - Smart Sister
// Model: claude-opus-5
//
// A number has the "productivity property" iff it is the product of the digits
// of some number. A product is positive only when every digit is 1..9, so the
// set of such numbers is exactly the set of products over {2,...,9} -- that is,
// the 7-smooth numbers 2^a * 3^b * 5^c * 7^d. Both directions hold: each digit
// factors into 2,3,5,7, and conversely any 2^a*3^b*5^c*7^d is spelled directly
// as a digit string of a 2s, b 3s, c 5s and d 7s. (Verified by brute force:
// the digit-products <= 100000 are exactly the 7-smooth numbers <= 100000.)
//
// So: tabulate the 7-smooth numbers in increasing order and answer each query
// by 1-based index. The statement guarantees answers below 10^18, which is
// index 66060; the table runs to 4*10^18 (74776 entries) so nothing near the
// bound can fall off the end.
//
// Ambiguity: "obtained by multiplying the digits of some OTHER number" reads as
// if a number may not be its own witness, but the restriction is vacuous --
// padding a witness with leading 1s leaves the product unchanged, so every
// candidate has a witness distinct from itself.
//
// Input framing: the statement gives a case count T, but the cases are read to
// EOF regardless, so a miscounted or truncated file still behaves.
//
// Submitted under language 0 (G++), not 4 (C++): the answers need 64-bit
// integers and POJ's C++ compiler has no long long. Output goes through
// iostream because MinGW's printf mishandles %llu.

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

typedef unsigned long long ull;

/* Generate every 7-smooth number <= LIM, i.e. 2^a * 3^b * 5^c * 7^d. */
static const ull LIM = 4000000000000000000ULL; /* > 10^18, < 2^63 */

static std::vector<ull> tab;

static void gen() {
    for (ull a = 1;; ) {
        for (ull b = a;; ) {
            for (ull c = b;; ) {
                for (ull d = c;; ) {
                    tab.push_back(d);
                    if (d > LIM / 7) break;
                    d *= 7;
                }
                if (c > LIM / 5) break;
                c *= 5;
            }
            if (b > LIM / 3) break;
            b *= 3;
        }
        if (a > LIM / 2) break;
        a *= 2;
    }
    std::sort(tab.begin(), tab.end());
}

/* buffered input: the statement warns about thousands of test cases */
static char ibuf[1 << 16];
static size_t ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen == 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

/* returns 0 on EOF, 1 on success */
static int readULL(ull *out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return 0;
    ull v = 0;
    while (c >= '0' && c <= '9') {
        v = v * 10 + (ull)(c - '0');
        c = gc();
    }
    *out = v;
    return 1;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    gen();
    ull t;
    if (!readULL(&t)) return 0; /* first token is the case count */
    ull idx;
    /* Read to EOF rather than trusting the count: extra cases are answered,
       a short file simply ends. Tokens outside the table are ignored. */
    while (readULL(&idx)) {
        if (idx >= 1 && idx <= (ull)tab.size()) {
            std::cout << tab[(size_t)idx - 1] << '\n';
        }
    }
    return 0;
}
