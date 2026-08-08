// POJ 2967 - Triangles
// Model: claude-opus-5
//
// A set is rejected when it hits either degenerate extreme: no three sticks
// form a triangle, or every three sticks form one.  So it is accepted iff at
// least one triple forms a triangle AND at least one triple does not.
// (Statement wording: "rejected if it breaks any of the following rules",
// where the two rules are mutually exclusive -- the reading above is the only
// one consistent with all three samples, and it makes every n<=3 set rejected.)
//
// With a[0]<=...<=a[n-1]:
//   * some triple fails the triangle inequality  <=>  a[0]+a[1] <= a[n-1]
//     (the smallest possible sum against the largest single stick);
//   * some triple forms a triangle  <=>  a[i]+a[i+1] > a[i+2] for some i
//     (any working triple i<j<k implies the consecutive triple k-2,k-1,k works).
// Degenerate a+b==c is not a triangle (sample 1: {1,2,3} is rejected).
//
// If no triple forms a triangle the sorted lengths grow at least as fast as
// Fibonacci, so with A[i] <= 2e9 such a set has at most 46 sticks.  Hence for
// n > 64 a triangle certainly exists and only the two smallest values and the
// maximum are needed -- no sort at all, O(n) total, one fread-based scan.
//
// All comparisons are written as subtractions (mx - m2 etc., always well
// defined since the subtrahend is the smaller value) so the sums never leave
// the exact range of 32-bit unsigned arithmetic.

#include <cstdio>
#include <algorithm>
using namespace std;

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static inline bool readUInt(unsigned int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return false;
    unsigned int x = 0;
    while (c >= '0' && c <= '9') {
        x = x * 10u + (unsigned int)(c - '0');
        c = gc();
    }
    out = x;
    return true;
}

static const unsigned int SMALL = 64u;
static unsigned int a[SMALL + 2];

int main() {
    unsigned int n;
    if (!readUInt(n)) return 0;

    if (n < 3u) {
        printf("The set is rejected.\n");
        return 0;
    }

    bool small = (n <= SMALL);
    unsigned int m1 = 0xFFFFFFFFu, m2 = 0xFFFFFFFFu, mx = 0u;
    for (unsigned int i = 0; i < n; ++i) {
        unsigned int v = 0;
        if (!readUInt(v)) break;
        if (small) a[i] = v;
        if (v < m1) { m2 = m1; m1 = v; }
        else if (v < m2) { m2 = v; }
        if (v > mx) mx = v;
    }

    bool hasNonTriangle = (m1 <= mx - m2);   // a[0] + a[1] <= a[n-1]

    bool hasTriangle;
    if (small) {
        sort(a, a + n);
        hasTriangle = false;
        for (unsigned int i = 0; i + 2u < n; ++i) {
            if (a[i] > a[i + 2] - a[i + 1]) { hasTriangle = true; break; }
        }
    } else {
        hasTriangle = true;
    }

    printf("%s\n", (hasNonTriangle && hasTriangle) ? "The set is accepted." : "The set is rejected.");
    return 0;
}
