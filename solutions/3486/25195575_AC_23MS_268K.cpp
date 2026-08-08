// POJ 3486 - Computers
// Model: claude-opus-5
//
// Cover years 1..n by consecutive ownership stretches.  Buying in year y and
// keeping the machine through year z costs c + m(y,z), so with
// f[z] = cheapest way to cover years 1..z,
//     f[0] = 0,  f[z] = min over y <= z of f[y-1] + c + m(y,z),
// and the answer is f[n].  The input hands the triangle row by row (y = 1..n,
// z = y..n), and f[y-1] is already final when row y starts, so each value is
// relaxed into f[z] as it is read: O(n^2) time, O(n) memory, no matrix stored.
//
// Two things the statement does not say, both confirmed on the discuss board
// and both checked here:
//  - m(y,z) is the TOTAL maintenance for owning the machine from year y
//    through year z, not a per-year figure.  "a maintenance cost m(y,z) each
//    year you own that computer" reads as per-year, but that reading gives 25
//    on the official sample instead of 19, so the sample settles it.
//  - The file holds MULTIPLE data sets and must be read to EOF.  The sample
//    has exactly one, so it is no evidence either way; a single-data-set
//    program reproduces the sample byte-for-byte.
// Whitespace is free-form ("white spaces can occur freely"), so input is
// tokenized by hand rather than read line by line.

#include <cstdio>
#include <vector>
#include <iostream>

static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

static int nextChar() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return (unsigned char)buf[bufPos++];
}

static bool readInt(long long &out) {
    int ch = nextChar();
    while (ch != -1 && (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' ||
                        ch == '\f' || ch == '\v'))
        ch = nextChar();
    if (ch == -1) return false;
    bool neg = false;
    if (ch == '-' || ch == '+') { neg = (ch == '-'); ch = nextChar(); }
    if (ch < '0' || ch > '9') return false;
    long long v = 0;
    while (ch >= '0' && ch <= '9') { v = v * 10 + (ch - '0'); ch = nextChar(); }
    out = neg ? -v : v;
    return true;
}

int main() {
    long long c, n;
    while (readInt(c)) {
        if (!readInt(n)) break;
        if (n < 0) n = 0;
        const long long INF = (long long)4e18;
        std::vector<long long> f((size_t)n + 1, INF);
        f[0] = 0;
        for (long long y = 1; y <= n; ++y) {
            long long base = f[(size_t)(y - 1)];
            for (long long z = y; z <= n; ++z) {
                long long m;
                if (!readInt(m)) m = 0;
                if (base != INF && base + c + m < f[(size_t)z])
                    f[(size_t)z] = base + c + m;
            }
        }
        std::cout << f[(size_t)n] << "\n";
    }
    return 0;
}
