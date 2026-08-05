// POJ 1364 - King
//
// Each of the m decisions constrains the sum of a contiguous block of the
// unknown integer sequence S = a_1..a_n. The question is only whether some
// integer sequence satisfies all of them: print "successful conspiracy" when
// none does (the conspirators win) and "lamentable kingdom" when one does.
// Note the outputs read backwards from intuition -- the "successful" line is
// the *infeasible* case.
//
// Statement reading that matters: S_i = {a_si, a_si+1, ..., a_si+ni} contains
// ni+1 terms, so with prefix sums P[k] = a_1 + ... + a_k (P[0] = 0) the sum is
// P[si+ni] - P[si-1]. The two prefix indices are si-1 and si+ni -- one wider
// on each end than the naive "si..si+ni" reading, and getting either end wrong
// silently shifts every constraint.
//
// That turns the input into a system of difference constraints on P[0..n].
// The inequalities are strict but everything is integral, so they tighten to
// non-strict ones:
//   sum <  k  =>  P[si+ni] - P[si-1] <= k-1     edge si-1 -> si+ni, w = k-1
//   sum >  k  =>  P[si-1] - P[si+ni] <= -(k+1)  edge si+ni -> si-1, w = -(k+1)
// A solution exists iff this graph has no negative cycle (Bellman-Ford), and
// the actual values of P are never needed.
//
// The graph can be disconnected -- nothing forces the constraints to touch a
// common index -- so the search must consider every node, not just those
// reachable from P[0]. That is done here by initialising every distance to 0,
// which is exactly a virtual source joined to all nodes by weight-0 edges
// (same mistake 3169's notes record). n, m <= 100 makes plain O(V*E)
// Bellman-Ford trivially fast.
//
// Distances are long long: a negative cycle keeps relaxing for V rounds before
// detection and each round can subtract |k|+1, which overflows int for large k
// even though no legitimate distance ever would.
#include <cstdio>
using namespace std;

const int MAXV = 512;
const int MAXE = 256;

struct Edge { int u, v; long long w; };
Edge es[MAXE];
long long dist_[MAXV];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int m;
        if (scanf("%d", &m) != 1) return 0;
        int ec = 0;
        int maxIdx = n;
        for (int i = 0; i < m; i++) {
            int s, ni, k; char op[8];
            scanf("%d %d %7s %d", &s, &ni, op, &k);
            long long kk = k;
            int lo = s - 1, hi = s + ni;
            if (hi > maxIdx) maxIdx = hi;
            if (lo < 0) lo = 0;
            if (op[0] == 'l') {
                // sum < k  =>  P[hi] - P[lo] <= k-1
                es[ec].u = lo; es[ec].v = hi; es[ec].w = kk - 1; ec++;
            } else {
                // sum > k  =>  P[hi] - P[lo] >= k+1  =>  P[lo] - P[hi] <= -(k+1)
                es[ec].u = hi; es[ec].v = lo; es[ec].w = -(kk + 1); ec++;
            }
        }
        int V = maxIdx + 1; // nodes 0..maxIdx
        for (int i = 0; i < V; i++) dist_[i] = 0; // == virtual source, 0-edges to all
        bool neg = false;
        for (int it = 0; it <= V; it++) {
            bool changed = false;
            for (int e = 0; e < ec; e++) {
                if (dist_[es[e].u] + es[e].w < dist_[es[e].v]) {
                    dist_[es[e].v] = dist_[es[e].u] + es[e].w;
                    changed = true;
                }
            }
            if (!changed) break;
            if (it == V) neg = true;
        }
        printf(neg ? "successful conspiracy\n" : "lamentable kingdom\n");
    }
    return 0;
}
