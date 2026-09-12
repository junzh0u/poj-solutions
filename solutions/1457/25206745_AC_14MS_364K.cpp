// POJ 1457 - Lord of the Ring
// Model: claude-sonnet-5
//
// This is the classic "turnpike reconstruction" (partial digest) problem:
// given the multiset of ALL pairwise distances between n points on a line,
// reconstruct the points (up to translation/reflection) and report the
// product of the gaps between adjacent points.
//
// From m = number of given distances we recover n via m = n*(n-1)/2; if no
// such integer n >= 2 exists, "No solution". Otherwise we run the standard
// backtracking placement algorithm (Skiena): the two extreme points are 0
// and width = max distance; repeatedly take the largest remaining distance
// y and try placing a new point at y or at width-y (it must be measured
// from one of the two already-fixed extremes), verifying that the distances
// from the candidate to every already-placed point are still available in
// the multiset. Placed-point positions must stay strictly inside (0,width)
// and distinct from existing points. Fail and backtrack otherwise; if every
// distance gets consumed with all n points placed, sort the points and
// multiply consecutive gaps for P.
//
// The discuss board (msg 2150) confirms the "all pairwise distances -> gap
// product" reading, and msg 99004 shows the well known pitfall of a search
// that doesn't consume every distance exactly once (which our count-based
// multiset structure with an exact remaining-count success check avoids).
// Board also notes POJ's official data for this problem is weak, so no
// pathological backtracking blowup is expected; a plain implementation of
// the standard algorithm suffices.
//
// Memory is tight (10000K), so distances are stored compressed as
// (distinct value, remaining count) pairs in plain arrays rather than in a
// std::multiset, which would blow the limit with its per-node overhead.
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;

static const int MAXM = 500050;
static const int MAXN = 1005;

int d[MAXM];
int vals[MAXM];
int cnt[MAXM];
int k;        // number of distinct values
int maxPtr;   // scan pointer for current max remaining value
int width;
int X[MAXN];
int xcnt;
int n;        // target number of points

bool takeValue(int v, int &idxOut) {
    int lo = 0, hi = k - 1, pos = -1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (vals[mid] == v) { pos = mid; break; }
        else if (vals[mid] < v) lo = mid + 1;
        else hi = mid - 1;
    }
    if (pos == -1 || cnt[pos] == 0) return false;
    cnt[pos]--;
    idxOut = pos;
    return true;
}

inline void giveValue(int idx) { cnt[idx]++; }

bool inX(int pos) {
    for (int i = 0; i < xcnt; i++) if (X[i] == pos) return true;
    return false;
}

bool place(int remaining);

// Stack of removed-value indices along the current DFS path. Each
// tryCandidate call pushes exactly `need` entries (right after whatever its
// caller/ancestors already pushed) and, if it backtracks, pops exactly that
// many back off -- true LIFO discipline matching the recursion, so this
// must NOT be a per-call static/local buffer (that would be silently
// shared and clobbered across nested recursive calls).
int stackIdx[MAXM];
int stackTop;

bool tryCandidate(int pos, int remaining) {
    if (pos <= 0 || pos >= width) return false;
    if (inX(pos)) return false;
    int need = xcnt;
    int base = stackTop;
    bool ok = true;
    for (int i = 0; i < need; i++) {
        int dist = abs(pos - X[i]);
        int idx;
        if (!takeValue(dist, idx)) { ok = false; break; }
        stackIdx[stackTop++] = idx;
    }
    if (!ok) {
        while (stackTop > base) giveValue(stackIdx[--stackTop]);
        return false;
    }
    X[xcnt++] = pos;
    if (place(remaining - need)) return true;
    xcnt--;
    while (stackTop > base) giveValue(stackIdx[--stackTop]);
    return false;
}

bool place(int remaining) {
    if (xcnt == n) return remaining == 0;
    while (maxPtr >= 0 && cnt[maxPtr] == 0) maxPtr--;
    if (maxPtr < 0) return false;
    int y = vals[maxPtr];
    int saved = maxPtr;
    if (tryCandidate(y, remaining)) return true;
    maxPtr = saved;
    int cand2 = width - y;
    if (cand2 != y) {
        if (tryCandidate(cand2, remaining)) return true;
        maxPtr = saved;
    }
    return false;
}

// fast integer reader
static inline int readInt(bool &ok) {
    int c = getchar();
    while (c != EOF && (c < '0' || c > '9') && c != '-') c = getchar();
    if (c == EOF) { ok = false; return 0; }
    bool neg = false;
    if (c == '-') { neg = true; c = getchar(); }
    long val = 0;
    while (c >= '0' && c <= '9') { val = val * 10 + (c - '0'); c = getchar(); }
    ok = true;
    return (int)(neg ? -val : val);
}

int main() {
    bool ok;
    while (true) {
        int m = readInt(ok);
        if (!ok) break;
        for (int i = 0; i < m; i++) d[i] = readInt(ok);
        if (m <= 0) { printf("No solution\n"); continue; }

        long mm = m;
        long approx = (long)((1.0 + sqrt(1.0 + 8.0 * (double)mm)) / 2.0 + 0.5);
        bool validN = false;
        long nn = 0;
        for (long cand = approx - 3; cand <= approx + 3; cand++) {
            if (cand >= 2 && cand * (cand - 1) / 2 == mm) { nn = cand; validN = true; break; }
        }
        if (!validN) { printf("No solution\n"); continue; }
        n = (int)nn;

        sort(d, d + m);
        k = 0;
        for (int i = 0; i < m; i++) {
            if (i == 0 || d[i] != d[i - 1]) { vals[k] = d[i]; cnt[k] = 1; k++; }
            else cnt[k - 1]++;
        }

        width = vals[k - 1];
        cnt[k - 1]--; // consume the max distance as the span
        maxPtr = k - 1;
        xcnt = 0;
        stackTop = 0;
        X[xcnt++] = 0;
        X[xcnt++] = width;

        bool solved = false;
        if (width > 0 || n == 2) {
            solved = place(m - 1);
        }

        if (solved) {
            sort(X, X + n);
            long P = 1;
            for (int i = 1; i < n; i++) P *= (long)(X[i] - X[i - 1]);
            printf("%ld\n", P);
        } else {
            printf("No solution\n");
        }
    }
    return 0;
}
