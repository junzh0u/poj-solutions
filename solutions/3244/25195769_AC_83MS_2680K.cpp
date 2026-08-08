// POJ 3244 - Difference between Triplets
// Model: claude-opus-5
//
// D(Ta,Tb) = max{dI,dJ,dK} - min{dI,dJ,dK}, i.e. the range of the three
// coordinate differences.  Range is translation invariant, so subtracting dK
// from all three gives range{dI-dK, dJ-dK, 0}.  With A = dI-dK and B = dJ-dK,
//
//     max(A,B,0) - min(A,B,0) = (|A| + |B| + |A-B|) / 2
//
// (both sides are positively homogeneous and piecewise linear, with pieces cut
// by the ordering of A, B, 0; verified exhaustively over every sign/order
// pattern).  Here A = d(I-K), B = d(J-K), A-B = d(I-J), so writing
// u = I-J, v = J-K, w = I-K for each triplet,
//
//     D(Ta,Tb) = ( |du| + |dv| + |dw| ) / 2
//
// and the answer is ( S(u) + S(v) + S(w) ) / 2 where S(x) is the sum of
// |xa - xb| over unordered pairs -- computed in O(N log N) by sorting and
// running prefix sums.  Each |.| has the parity of its argument, so the total
// is always even and the halving is exact.
//
// Magnitudes: u,v,w lie in [-2e6, 2e6], so each S is at most about 4e16 and
// the answer at most 4e16 (attained by two antipodal halves).  That is past
// 2^53, which is the discuss board's "double fails, __int64 passes" -- so this
// is 64-bit integer throughout.  Submitted as G++ (language 0) for long long.
//
// Input framing: the statement gives an explicit N per case and a 0 sentinel,
// both honored; N=1 correctly yields 0.

#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

const int MAXN = 200005;

static int u[MAXN], v[MAXN], w[MAXN];

static const int BUFSZ = 1 << 16;
static char ibuf[BUFSZ];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, BUFSZ, stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos++];
}

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sign;
    return true;
}

/* Sum of |a[i] - a[j]| over all unordered pairs. */
static long long pairSum(int *a, int n) {
    sort(a, a + n);
    long long total = 0, prefix = 0;
    for (int i = 0; i < n; i++) {
        total += (long long)i * a[i] - prefix;
        prefix += a[i];
    }
    return total;
}

int main() {
    int n;
    while (readInt(n) && n != 0) {
        for (int i = 0; i < n; i++) {
            int I, J, K;
            readInt(I); readInt(J); readInt(K);
            u[i] = I - J;
            v[i] = J - K;
            w[i] = I - K;
        }
        long long ans = (pairSum(u, n) + pairSum(v, n) + pairSum(w, n)) / 2;
        cout << ans << "\n";
    }
    return 0;
}
