// POJ 3385 - Genealogy
// Model: claude-opus-5
//
// Input gives, for each alien i in 1..n, the identifier of its child c[i]
// (Peter is 0).  Reversing the pointers, the parents of a vertex v are exactly
// the i with c[i] == v, so the whole task depends only on the in-degrees
// k(v) = #{ i : c[i] == v } for every v.
//
// A vertex with k parents may be repaired by inserting x new ancestors between
// it and (some of) its parents; every original parent-child edge must survive
// as an ancestor-descendant relation, so the k parents plus the x new vertices
// all have to be attached below v inside that gadget.  That is k + x items
// filling the d slots of v and the d slots of each new vertex, i.e.
// k + x <= d * (x + 1), so x >= ceil((k - d) / (d - 1)); the bound is
// achievable (place the new vertices greedily, then the parents), hence
//   f(k) = ceil((k - d) / (d - 1)) = (k - 2) / (d - 1)  when k > d, else 0.
// The answer is the sum of f(k(v)) over all v, vertex 0 included -- Peter is
// an alien too and is also limited to d parents.
//
// The gadgets of distinct vertices are independent (an inserted ancestor has a
// single child, so it can only serve one v), so summing is optimal.
//
// No ambiguity in the statement; the sample is reproduced by the figures.
// The sample is weak, though: with d = 2 the ceiling is exact and no vertex has
// exactly one parent, so both the missing k > d guard and floor-instead-of-ceil
// survive it.  Both are covered by hand-built cases instead.
//
// Notes on robustness: input is read to EOF as a sequence of cases (a single
// case then just prints one line), identifiers outside the direct-indexed range
// fall back to a std::map instead of being dropped or indexing out of bounds,
// and only the touched counters are cleared between cases.
// The discuss board reports this problem TLE-ing under G++ while the same code
// runs in ~225ms under C++, and blames MinGW stream I/O; the input is read with
// one fread and a hand-rolled scanner, which sidesteps that entirely.

#include <cstdio>
#include <cstdlib>
#include <map>

static const int CAP = 1 << 20; /* counter range, well past the stated n */

static int cnt[CAP];
static int touched[CAP];

static char *buf;
static size_t bufLen, bufPos;

static void readAll(void) {
    size_t cap = 1 << 20, len = 0, r;
    buf = (char *)malloc(cap);
    while ((r = fread(buf + len, 1, cap - len, stdin)) > 0) {
        len += r;
        if (len == cap) {
            cap <<= 1;
            buf = (char *)realloc(buf, cap);
        }
    }
    bufLen = len;
    bufPos = 0;
}

/* Returns 0 at end of input, 1 on success. */
static int readInt(int *out) {
    int sign = 1;
    unsigned long v;
    while (bufPos < bufLen &&
           (buf[bufPos] < '0' || buf[bufPos] > '9') && buf[bufPos] != '-')
        ++bufPos;
    if (bufPos >= bufLen) return 0;
    if (buf[bufPos] == '-') { sign = -1; ++bufPos; }
    v = 0;
    while (bufPos < bufLen && buf[bufPos] >= '0' && buf[bufPos] <= '9') {
        if (v < 200000000UL) v = v * 10 + (unsigned long)(buf[bufPos] - '0');
        ++bufPos;
    }
    *out = sign * (int)v;
    return 1;
}

int main(void) {
    int n, d;
    readAll();
    while (readInt(&n) && readInt(&d)) {
        std::map<int, int> far; /* identifiers outside the direct-indexed range */
        std::map<int, int>::const_iterator it;
        int nTouched = 0, i, ans = 0;
        for (i = 0; i < n; ++i) {
            int c;
            if (!readInt(&c)) break;
            if (c < 0 || c >= CAP) { ++far[c]; continue; }
            if (cnt[c] == 0) touched[nTouched++] = c;
            ++cnt[c];
        }
        for (i = 0; i < nTouched; ++i) {
            int k = cnt[touched[i]];
            if (k > d) ans += (k - 2) / (d - 1);
            cnt[touched[i]] = 0;
        }
        for (it = far.begin(); it != far.end(); ++it)
            if (it->second > d) ans += (it->second - 2) / (d - 1);
        printf("%d\n", ans);
    }
    return 0;
}
