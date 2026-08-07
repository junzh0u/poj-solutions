// POJ 2680 - Computer Transformation
// Model: claude-opus-5
//
// The string after n steps is the Thue-Morse word (0 -> 10, 1 -> 01, seed "1").
// Let a(n) be the number of adjacent "00" pairs after n steps.
//
// A "00" can only be produced across a substitution boundary: within one step a
// 0 expands to "10" and a 1 to "01", so a new "00" appears exactly where the
// previous string had "01" (the 0 contributes ...0 as its second char, the 1
// contributes 0... as its first char).  Hence a(n) = #"01" in step n-1.  Counting
// "01" occurrences the same way gives the linear recurrence
//
//     a(n) = a(n-1) + 2*a(n-2)   for n >= 3,   a(1) = 0, a(2) = 1
//
// (equivalently a(n) = a(n-2) + 2^(n-3)).  Both forms were checked against a
// direct string simulation for n = 0..22; the recurrence only starts at n = 3,
// since a(2) = 1 while a(1) + 2*a(0) = 0.
//
// Thue-Morse contains no "000", so overlapping vs. non-overlapping counting of
// "00" is the same question and the statement's wording is unambiguous here.
//
// n <= 1000 makes a(n) ~ 2^n/6, about 300 decimal digits, so big integers are
// required.  All values are precomputed once (base 10^4 limbs) and each query
// is answered by a lookup.  Input is a stream of numbers read to EOF.
#include <cstdio>
#include <vector>

using namespace std;

static const unsigned long BASE = 10000ul; // small enough that a + 2*b + carry
                                           // never leaves unsigned long, so the
                                           // code stays strict C++98 (no long long)

// c = a + 2*b, little-endian base-10000 limbs
static void addTwice(const vector<unsigned long> &a, const vector<unsigned long> &b,
                     vector<unsigned long> &c) {
    size_t n = a.size() > b.size() ? a.size() : b.size();
    c.assign(n, 0ul);
    unsigned long carry = 0ul;
    for (size_t i = 0; i < n; ++i) {
        unsigned long cur = carry;
        if (i < a.size()) cur += a[i];
        if (i < b.size()) cur += 2ul * b[i];
        c[i] = cur % BASE;
        carry = cur / BASE;
    }
    while (carry) {
        c.push_back(carry % BASE);
        carry /= BASE;
    }
    while (c.size() > 1 && c[c.size() - 1] == 0ul) c.erase(c.end() - 1);
}

int main() {
    const int MAXN = 1000;
    vector< vector<unsigned long> > f(MAXN + 1);
    f[0].assign(1, 0ul);
    f[1].assign(1, 0ul);
    f[2].assign(1, 1ul);
    for (int i = 3; i <= MAXN; ++i) addTwice(f[i - 1], f[i - 2], f[i]);

    int n;
    while (scanf("%d", &n) == 1) {
        if (n < 0) n = 0;
        if (n > MAXN) n = MAXN;
        const vector<unsigned long> &v = f[n];
        printf("%lu", v[v.size() - 1]);
        for (int i = (int)v.size() - 2; i >= 0; --i) printf("%04lu", v[i]);
        printf("\n");
    }
    return 0;
}
