// POJ 1275 - Cashier Employment
//
// N applicants each work a fixed 8-hour shift starting at a specified hour
// t_i (0..23), wrapping past midnight. Hour h needs at least R(h) cashiers
// on duty. Hire the fewest applicants meeting every hour's requirement, or
// report "No Solution" if no subset of applicants can.
//
// Group applicants by start hour: num[i] = count of applicants with t_i = i.
// x[i] = number hired starting at hour i is the only thing that matters
// (which specific applicant is irrelevant), 0 <= x[i] <= num[i]. Hour h is
// covered by every start i with i <= h <= i+7 (mod 24), i.e. i in the 8
// values {h-7, ..., h} mod 24, so the constraint is a *circular* sum of a
// contiguous block of x -- the same difference-constraint shape as 3169,
// 1364 and 2983 in this repo, but the wrap means the block's length in the
// unrolled prefix-sum array depends on the (unknown) total hired S.
//
// Fix S = sum x[i] and build prefix sums P[0..24] (P[i+1]-P[i] = x[i]).
// For h >= 7 the covering block [h-7, h] doesn't wrap: P[h+1]-P[h-7] >= R(h).
// For h < 7 it wraps around midnight into [17+h, 23] u [0, h], and using
// P[24]-P[0] = S (enforced separately) that becomes
// P[h+1] - P[17+h] >= R(h) - S. Both, plus 0 <= x[i] <= num[i] and the
// P[24]-P[0] = S tie, are difference constraints P[v]-P[u] <= w, i.e. an
// edge u->v weight w; feasibility for a given S is "no negative cycle"
// (Bellman-Ford from a virtual source realized as dist[i]=0 for all i, as
// in 3169/1364/2983 -- here the P-chain already connects every node, so it
// isn't strictly needed, but it's free and keeps the pattern uniform).
//
// S is unknown, so enumerate it. Feasibility is monotone non-decreasing in
// S on [0, N]: given a feasible x for total S < N, some x[i] < num[i]
// (since sum num[i] = N > S), and bumping that x[i] by 1 keeps every bound
// and can only help the ">=" coverage constraints, giving a feasible S+1.
// So the smallest feasible S is found by a linear scan from 0, stopping at
// the first success; if S = N itself is infeasible, nothing is (No
// Solution). With only 25 prefix nodes and <80 edges per check this scan
// (up to N+1 <= 1001 Bellman-Ford runs per case, 20 cases) is fast enough
// with room to spare against the 1000ms limit.
#include <cstdio>
using namespace std;

struct Edge { int u, v; long long w; };

int R[24];
int num[24];

bool feasible(int S, Edge* es) {
    int ec = 0;
    for (int i = 0; i < 24; i++) {
        es[ec].u = i+1; es[ec].v = i; es[ec].w = 0; ec++; // x[i]>=0
        es[ec].u = i; es[ec].v = i+1; es[ec].w = num[i]; ec++; // x[i]<=num[i]
    }
    for (int h = 0; h < 24; h++) {
        int lo = h - 7;
        if (lo >= 0) {
            es[ec].u = h+1; es[ec].v = lo; es[ec].w = -R[h]; ec++;
        } else {
            int lo2 = 24 + lo;
            es[ec].u = h+1; es[ec].v = lo2; es[ec].w = (long long)S - R[h]; ec++;
        }
    }
    es[ec].u = 0; es[ec].v = 24; es[ec].w = S; ec++;
    es[ec].u = 24; es[ec].v = 0; es[ec].w = -S; ec++;

    const int V = 25;
    static long long dist_[25];
    for (int i = 0; i < V; i++) dist_[i] = 0;
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
    return !neg;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    Edge es[100];
    while (T--) {
        for (int i = 0; i < 24; i++) scanf("%d", &R[i]);
        int N;
        scanf("%d", &N);
        for (int i = 0; i < 24; i++) num[i] = 0;
        for (int i = 0; i < N; i++) {
            int t; scanf("%d", &t);
            num[t]++;
        }
        int ans = -1;
        for (int S = 0; S <= N; S++) {
            if (feasible(S, es)) { ans = S; break; }
        }
        if (ans == -1) printf("No Solution\n");
        else printf("%d\n", ans);
    }
    return 0;
}
