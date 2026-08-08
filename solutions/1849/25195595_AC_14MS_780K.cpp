// POJ 1849 - Two
// Model: claude-opus-5
//
// Two snow plovers start at S on a weighted tree and must together traverse every
// edge at least once (re-driving a cleared street still burns fuel); each parks
// wherever its walk ends.  Minimise total metres driven.
//
// Answer = 2 * (sum of all edge weights) - (tree diameter), independent of S.
//
// Lower bound: a walk from S covering a connected edge set E and ending at t costs
// at least 2*w(E) - d(S,t).  With sets E1, E2 whose union is everything,
//   cost >= 2*(w(E1)+w(E2)) - d(S,t1) - d(S,t2) >= 2*W + 2*w(E1^E2) - d(S,t1) - d(S,t2).
// Both Ei contain the path S->ti, so E1^E2 contains their common prefix S->x, where
// x is the vertex at which the two paths diverge.  Substituting d(S,ti)=d(S,x)+d(x,ti)
// the d(S,x) terms cancel exactly and the bound becomes 2*W - d(x,t1) - d(x,t2),
// which is at least 2*W - diameter.  S drops out of the expression entirely.
// Achievable: let x be the vertex where S's path meets a diameter path t1..t2; both
// plovers drive S->x, then split, one to each diameter end, with every hanging
// subtree assigned to whichever plover already reaches its attachment vertex.
//
// So S is a red herring: the natural reading "sum of the two deepest branches
// hanging off S" is wrong, and the official sample cannot expose it -- that sample
// is a path with S on it, where the two branch depths at S add up to the diameter.
// Separating case: 4 vertices, S=1, edges 1-2(1), 2-3(10), 2-4(10).  S is a leaf, so
// the S-based reading saves only 11 and answers 31, while both plovers driving the
// stem and taking one arm each costs 22 = 2*21 - 20.
//
// Diameter by the usual two BFS sweeps, iterative (N = 100000 admits a path-shaped
// tree, so recursion would risk a stack overflow).  Every quantity is bounded by
// 2 * 200004 * 1000 < 2^31, so int suffices.  The input is read to EOF so that a
// file holding several data sets is handled as well as a single one.

#include <cstdio>
#include <iostream>

static const int MAXN = 200005;
static const int MAXE = 2 * MAXN;

static int ea[MAXN], eb[MAXN], ec[MAXN];
static int head[MAXN], nxt[MAXE], to_[MAXE], wt[MAXE];
static int dist_[MAXN], que[MAXN];

/* BFS over the tree from src; returns the farthest vertex, sets *best to its distance. */
static int bfsFar(int src, int nv, int *best) {
    int i;
    for (i = 1; i <= nv; ++i) dist_[i] = -1;
    int qh = 0, qt = 0;
    dist_[src] = 0;
    que[qt++] = src;
    int far = src, fd = 0;
    while (qh < qt) {
        int u = que[qh++];
        if (dist_[u] > fd) { fd = dist_[u]; far = u; }
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to_[e];
            if (dist_[v] < 0) { dist_[v] = dist_[u] + wt[e]; que[qt++] = v; }
        }
    }
    *best = fd;
    return far;
}

static int clampV(int v) {
    if (v < 1) return 1;
    if (v > MAXN - 1) return MAXN - 1;
    return v;
}

int main() {
    int n, s;
    while (scanf("%d %d", &n, &s) == 2) {
        int i, m = 0, total = 0;
        int maxv = n < 1 ? 1 : n;
        for (i = 0; i < n - 1 && m < MAXN - 1; ++i) {
            int a, b, c;
            if (scanf("%d %d %d", &a, &b, &c) != 3) break;
            a = clampV(a); b = clampV(b);
            ea[m] = a; eb[m] = b; ec[m] = c; ++m;
            if (a > maxv) maxv = a;
            if (b > maxv) maxv = b;
            total += c;
        }
        for (i = 1; i <= maxv; ++i) head[i] = -1;
        int ecnt = 0;
        for (i = 0; i < m; ++i) {
            to_[ecnt] = eb[i]; wt[ecnt] = ec[i]; nxt[ecnt] = head[ea[i]]; head[ea[i]] = ecnt; ++ecnt;
            to_[ecnt] = ea[i]; wt[ecnt] = ec[i]; nxt[ecnt] = head[eb[i]]; head[eb[i]] = ecnt; ++ecnt;
        }
        s = clampV(s);
        if (s > maxv) s = 1;
        int d1 = 0, diam = 0;
        int a = bfsFar(s, maxv, &d1);
        bfsFar(a, maxv, &diam);
        std::cout << (2 * total - diam) << "\n";
    }
    return 0;
}
