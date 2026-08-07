// POJ 2526 - Center of symmetry
// Model: claude-opus-5
//
// Decide whether a set S of n distinct lattice points has a centre of symmetry:
// some point s (not necessarily in S, not necessarily a lattice point) with
// 2s - p in S for every p in S.
//
// The map r(p) = 2s - p is injective, sends S into S, and S is finite, so it is
// an involution of S.  It is also strictly order-reversing for the lexicographic
// order on (x, y), since both coordinates are negated.  Hence, with the points
// sorted lexicographically, r must send a[i] to a[n-1-i], i.e. the set is
// centrally symmetric exactly when a[i] + a[n-1-i] is the same vector for all i.
// Sort, then compare each mirrored pair against a[0] + a[n-1].  O(n log n).
//
// Ambiguity settled from the statement's wording (and confirmed on the discuss
// board): nothing forbids q = p, so a point may be its own mirror.  Odd n is
// therefore legal - the middle element of the sorted array is then the centre
// itself, which the i <= j loop checks as 2*a[mid] == a[0] + a[n-1].  n = 1 is
// "yes".  s may be half-integral, so the test is on the doubled centre
// a[i] + a[n-1-i] and never divides; coordinates are at most 1e7 in absolute
// value, so those sums fit comfortably in int and no 64-bit type is needed.
//
// Input framing: the leading count c is read but the cases are consumed until
// EOF (or a non-positive n), which agrees with c on well-formed input and
// survives the "extra data set" framing the board warns about.  All per-case
// state is re-derived from n points each round - the board's most reported
// wrong answer here is stale state carried between data sets.

#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct Pt {
    int x, y;
};

static bool lessPt(const Pt &a, const Pt &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

static const int BUFSZ = 1 << 16;
static char buf[BUFSZ];
static int bufLen = 0, bufPos = 0;

static int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, BUFSZ, stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return (unsigned char)buf[bufPos++];
}

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && c != '-' && (c < '0' || c > '9')) c = gc();
    if (c == -1) return false;
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sign;
    return true;
}

static const int MAXN = 10005;
static Pt pts[MAXN];

int main() {
    int cases;
    if (!readInt(cases)) return 0;
    int n;
    while (readInt(n) && n > 0) {
        if (n > MAXN - 1) n = MAXN - 1; /* defensive; statement caps n at 10000 */
        for (int i = 0; i < n; i++) {
            readInt(pts[i].x);
            readInt(pts[i].y);
        }
        sort(pts, pts + n, lessPt);
        int sx = pts[0].x + pts[n - 1].x;
        int sy = pts[0].y + pts[n - 1].y;
        bool ok = true;
        for (int i = 0, j = n - 1; i <= j; i++, j--) {
            if (pts[i].x + pts[j].x != sx || pts[i].y + pts[j].y != sy) { ok = false; break; }
        }
        puts(ok ? "yes" : "no");
    }
    return 0;
}
