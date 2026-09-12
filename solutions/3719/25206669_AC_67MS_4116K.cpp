// POJ 3719 - Art of Balance
// Model: claude-sonnet-5
//
// The skeleton is a full binary tree; leaves get a bijective assignment of
// the M given weights.  For an internal node whose left/right subtrees carry
// total weight L and R, the fulcrum shifts so that L*d_left = R*d_right with
// d_left+d_right = 1000, giving adjusted distance 500*|L-R|/(L+R).  We must
// choose the weight-to-leaf assignment minimizing the sum of this quantity
// over all internal nodes.
//
// Since it is guaranteed leaves == M <= 16, the tree has at most 2*M-1 <= 31
// nodes.  DP over bitmasks of the M weight indices: dp[node][S] = minimum
// total cost achievable in node's subtree when exactly the weight-index set S
// (|S| = leaf count of node) is distributed among its leaves.  For an
// internal node with children L (kL leaves) and R (kR leaves):
//   dp[node][S] = min over A subset of S, |A|=kL, B=S\A of
//                 dp[L][A] + dp[R][B] + 500*|sumW(A)-sumW(B)|/(sumW(A)+sumW(B))
// Leaves: dp[leaf][{i}] = 0 for every single index i (any weight could end up
// there depending on the global assignment).
//
// Enumerating (node mask S, left submask A) rather than both children's
// masks independently is what the discuss board flags as the fix for TLE.
// To go further, submasks A of S with |A| = kL are generated directly via
// Gosper's hack over a dense kL-of-kv combination index (mapped back onto
// S's actual bit positions), instead of walking all 2^|S| submasks of S and
// discarding the wrong-popcount ones - this turns the per-node cost into
// C(M,kv)*C(kv,kL) rather than C(M,kv)*2^kv, i.e. a multinomial sum instead
// of a 3^M blow-up in the worst (near-chain) tree shape.
//
// N <= 100 is a loose bound; because leaves == M <= 16 and every internal
// node has exactly two children (per the input format), N is actually at
// most 2*M-1 <= 31 (confirmed on the discuss board).
//
// No ambiguity in the statement itself; the board mainly confirms M<=16 and
// the small real bound on N.

#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
using namespace std;

int N, M;
int lc[105], rc[105];
bool isLeaf[105];
int wgt[20];
int leafCnt[105];

double sumWArr[1 << 16];
int popc[1 << 16];
int bitIndex[1 << 16]; // for powers of two: index of the set bit

void computeLeafCnt(int u) {
    if (isLeaf[u]) { leafCnt[u] = 1; return; }
    computeLeafCnt(lc[u]);
    computeLeafCnt(rc[u]);
    leafCnt[u] = leafCnt[lc[u]] + leafCnt[rc[u]];
}

inline unsigned nextComb(unsigned x) {
    unsigned u = x & (-x);
    unsigned v = u + x;
    return v + (((v ^ x) / u) >> 2);
}

vector<double> solve(int u) {
    int full = 1 << M;
    if (isLeaf[u]) {
        vector<double> res(full, -1.0); // -1 marks "unused" mask
        for (int i = 0; i < M; i++) res[1 << i] = 0.0;
        return res;
    }
    vector<double> L = solve(lc[u]);
    vector<double> R = solve(rc[u]);
    int kL = leafCnt[lc[u]], kR = leafCnt[rc[u]], kv = kL + kR;
    vector<double> res(full, -1.0);
    int pos[16];
    for (int S = 0; S < full; S++) {
        if (popc[S] != kv) continue;
        int cnt = 0;
        int t = S;
        while (t) {
            int lb = t & (-t);
            pos[cnt++] = bitIndex[lb];
            t ^= lb;
        }
        double best = -1.0;
        unsigned limit = 1u << kv;
        unsigned c = (1u << kL) - 1;
        while (c < limit) {
            int A = 0;
            unsigned cc = c;
            while (cc) {
                int lb = cc & (-cc);
                A |= (1 << pos[bitIndex[lb]]);
                cc ^= lb;
            }
            int B = S ^ A;
            double lv = L[A], rv = R[B];
            if (lv >= 0.0 && rv >= 0.0) {
                double sA = sumWArr[A], sB = sumWArr[B];
                double cost = 500.0 * fabs(sA - sB) / (sA + sB);
                double tot = lv + rv + cost;
                if (best < 0.0 || tot < best) best = tot;
            }
            c = nextComb(c);
        }
        res[S] = best;
    }
    return res;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d", &N);
        for (int i = 1; i <= N; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            if (a == -1 && b == -1) {
                isLeaf[i] = true;
            } else {
                isLeaf[i] = false;
                lc[i] = a; rc[i] = b;
            }
        }
        scanf("%d", &M);
        for (int i = 0; i < M; i++) scanf("%d", &wgt[i]);

        computeLeafCnt(1);

        int full = 1 << M;
        popc[0] = 0;
        for (int i = 1; i < full; i++) popc[i] = popc[i >> 1] + (i & 1);
        for (int i = 0; i < M; i++) bitIndex[1 << i] = i;
        sumWArr[0] = 0.0;
        for (int i = 1; i < full; i++) {
            int lb = i & (-i);
            sumWArr[i] = sumWArr[i ^ lb] + wgt[bitIndex[lb]];
        }

        vector<double> ans = solve(1);
        printf("%.3f\n", ans[full - 1]);
    }
    return 0;
}
