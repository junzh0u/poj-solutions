// POJ 3585 - Accumulation Degree
// Model: claude-opus-5
//
// A(x) is the max flow from x to every *other* terminal (a terminal is a node
// of degree 1).  On a tree the branches out of a node are independent, so the
// flow that can leave x through an incident edge of capacity c to neighbour y
// is min(c, how much the y-side can swallow) -- and the y-side swallows
// everything up to c if y is itself a terminal, otherwise only what it can push
// on through its own remaining edges.  That makes A() a rerooting DP:
//
//   d[x] = sum over children y of ( deg[y]==1 ? c(x,y) : min(c(x,y), d[y]) )
//   f[root] = d[root]
//   f[y]  = d[y] + ( deg[x]==1 ? c : min(c, f[x] - contribution of y to d[x]) )
//
// where x is y's parent.  The two deg==1 tests are the whole problem: a leaf
// absorbs the full capacity of its single edge because it is a sink, not a
// relay.  The subtracted term must be y's *actual* contribution to d[x], which
// is c when y is a leaf and min(c, d[y]) otherwise -- not min(c, d[y]) in both
// cases.  Answer = max over x of f[x].
//
// Verified against an independent Dinic max flow (source x, super sink fed by
// every terminal != x, each tree edge modelled as capacity c in both
// directions): it reproduces all five A() values the statement spells out for
// its figure, and agrees with this DP on 900 random small trees (paths, stars,
// caterpillars, random shapes, capacities up to 200000).  The discuss board's
// two published traps -- "3 / 1 2 12 / 2 3 13" -> 25 and "2 / 1 2 3" -> 3 --
// both come out right.
//
// Notes on the limits: n and the capacities both go to 200000, so a star gives
// answers around 4e10 and long long is required (an int overflow here is
// silent).  Traversal is an explicit BFS order rather than recursion -- a
// 200000-node chain would blow the judge's stack, which is what the board's
// Runtime Errors are.  Output goes through cout so no long-long printf format
// specifier (%lld vs %I64d) is involved.

#include <cstdio>
#include <iostream>

using namespace std;

const int MAXN = 200005;
const int MAXE = 2 * MAXN;

static int head[MAXN], nxt[MAXE], to_[MAXE], wt[MAXE];
static int deg[MAXN];
static int order_[MAXN], par[MAXN];
static long long d[MAXN], f[MAXN];

static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

static int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return buf[bufPos++];
}

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sgn;
    return true;
}

int main() {
    int T;
    if (!readInt(T)) return 0;
    while (T-- > 0) {
        int n;
        if (!readInt(n)) break;
        int i;
        for (i = 1; i <= n; ++i) { head[i] = -1; deg[i] = 0; }
        int ec = 0;
        for (i = 0; i < n - 1; ++i) {
            int x, y, z;
            readInt(x); readInt(y); readInt(z);
            to_[ec] = y; wt[ec] = z; nxt[ec] = head[x]; head[x] = ec; ++ec;
            to_[ec] = x; wt[ec] = z; nxt[ec] = head[y]; head[y] = ec; ++ec;
            ++deg[x]; ++deg[y];
        }

        // BFS from node 1; order_ is a valid topological order of the rooted tree
        int cnt = 0;
        order_[cnt++] = 1;
        par[1] = 0;
        for (i = 0; i < cnt; ++i) {
            int x = order_[i];
            for (int e = head[x]; e != -1; e = nxt[e]) {
                int y = to_[e];
                if (y == par[x]) continue;
                par[y] = x;
                order_[cnt++] = y;
            }
        }

        // d[x]: max flow from x into its own subtree
        for (i = cnt - 1; i >= 0; --i) {
            int x = order_[i];
            long long s = 0;
            for (int e = head[x]; e != -1; e = nxt[e]) {
                int y = to_[e];
                if (y == par[x]) continue;
                if (deg[y] == 1) s += wt[e];
                else s += (d[y] < wt[e] ? d[y] : (long long)wt[e]);
            }
            d[x] = s;
        }

        // reroot: f[x] == A(x)
        long long ans = 0;
        f[1] = d[1];
        if (f[1] > ans) ans = f[1];
        for (i = 0; i < cnt; ++i) {
            int x = order_[i];
            for (int e = head[x]; e != -1; e = nxt[e]) {
                int y = to_[e];
                if (y == par[x]) continue;
                long long c = wt[e];
                long long contrib = (deg[y] == 1) ? c : (d[y] < c ? d[y] : c);
                long long up;
                if (deg[x] == 1) up = c;   // x is a terminal: it absorbs the whole edge
                else {
                    long long rest = f[x] - contrib;
                    up = (rest < c ? rest : c);
                    if (up < 0) up = 0;
                }
                f[y] = d[y] + up;
                if (f[y] > ans) ans = f[y];
            }
        }

        cout << ans << "\n";
    }
    return 0;
}
