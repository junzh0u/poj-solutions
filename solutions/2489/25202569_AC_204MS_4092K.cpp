// POJ 2489 - Line Segments
// Model: claude-sonnet-5
// Approach: For each segment, compute a canonical exact-integer line key
// (normalized (A,B,C) with A*x+B*y=C, direction reduced by gcd with a fixed
// sign convention) so segments on the same infinite line hash/sort together
// with no floating point (the board's message 140128 reports WA from using
// double slopes for exactly this grouping). Within each line group, project
// each segment onto a 1D coordinate (x if the line isn't vertical, else y)
// to get an interval [lo,hi]. Two collinear segments "overlap" (intersect in
// infinitely many points) exactly when their intervals share more than a
// single point, i.e. after sorting by lo ascending, interval j (processed
// later) overlaps an earlier interval i iff hi_i > lo_j (verified against
// the sample: touching at a single shared endpoint, e.g. (10,20)-(20,30),
// must NOT count). Counting such pairs per group is done by sorting by lo,
// then for each interval querying (with a Fenwick tree over coordinate-
// compressed hi values) how many previously-inserted hi's exceed the
// current lo, then inserting the current hi. Overall O(n log n) per
// scenario. No statement ambiguity beyond the overlap definition, which the
// discuss board's quoted problem text and the worked sample both pin down.
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

typedef long long ll;

const int MAXN = 100005;

struct Seg {
    ll A, B, C;
    int lo, hi;
};

Seg segs[MAXN];
int bitArr[MAXN];
int bitSize;
int hiVals[MAXN];

static inline int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) c = getchar();
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sign;
}

ll gcdll(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

bool cmpSeg(const Seg &a, const Seg &b) {
    if (a.A != b.A) return a.A < b.A;
    if (a.B != b.B) return a.B < b.B;
    if (a.C != b.C) return a.C < b.C;
    return a.lo < b.lo;
}

void bitReset(int sz) {
    bitSize = sz;
    for (int i = 0; i <= sz; i++) bitArr[i] = 0;
}

void bitUpdate(int i) {
    for (; i <= bitSize; i += i & (-i)) bitArr[i]++;
}

int bitQuery(int i) {
    int s = 0;
    for (; i > 0; i -= i & (-i)) s += bitArr[i];
    return s;
}

int main() {
    int T = readInt();
    for (int tc = 1; tc <= T; tc++) {
        int n = readInt();
        for (int i = 0; i < n; i++) {
            int x1 = readInt(), y1 = readInt(), x2 = readInt(), y2 = readInt();
            ll dx = x2 - x1, dy = y2 - y1;
            ll g = gcdll(dx, dy);
            ll ndx = dx / g, ndy = dy / g;
            if (ndx < 0 || (ndx == 0 && ndy < 0)) { ndx = -ndx; ndy = -ndy; }
            ll A = ndy, B = -ndx;
            ll C = A * (ll)x1 + B * (ll)y1;
            segs[i].A = A; segs[i].B = B; segs[i].C = C;
            if (x1 != x2) {
                segs[i].lo = min(x1, x2);
                segs[i].hi = max(x1, x2);
            } else {
                segs[i].lo = min(y1, y2);
                segs[i].hi = max(y1, y2);
            }
        }
        sort(segs, segs + n, cmpSeg);
        ll total = 0;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && segs[j].A == segs[i].A && segs[j].B == segs[i].B && segs[j].C == segs[i].C) j++;
            int m = j - i;
            if (m > 1) {
                for (int k = 0; k < m; k++) hiVals[k] = segs[i + k].hi;
                sort(hiVals, hiVals + m);
                int cnt = (int)(unique(hiVals, hiVals + m) - hiVals);
                bitReset(cnt);
                int inserted = 0;
                for (int k = 0; k < m; k++) {
                    int lo = segs[i + k].lo;
                    int hi = segs[i + k].hi;
                    int idxLE = (int)(upper_bound(hiVals, hiVals + cnt, lo) - hiVals);
                    int leCount = bitQuery(idxLE);
                    int gt = inserted - leCount;
                    total += gt;
                    int rank = (int)(lower_bound(hiVals, hiVals + cnt, hi) - hiVals) + 1;
                    bitUpdate(rank);
                    inserted++;
                }
            }
            i = j;
        }
        printf("Scenario #%d:\n%lld\n\n", tc, total);
    }
    return 0;
}
