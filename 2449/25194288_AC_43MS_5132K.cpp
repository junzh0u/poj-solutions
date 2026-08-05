// POJ 2449 - Remmarguts' Date
//
// K-th shortest walk from S to T in a directed graph (N<=1000, M<=100000, K<=1000);
// walks may revisit stations, and equal-length walks count as different paths.
//
// Statement ambiguity: when S == T the prince still has to travel, so the empty
// zero-length walk is not the first path. The search naturally emits it first, so
// K is bumped by one in that case.
//
// Approach. Run Dijkstra on the reversed graph from T to get h[v], the shortest
// distance v -> T; h[S] == INF means no path at all, print -1. h is a consistent
// heuristic, so ordering candidates by f = g + h[v] enumerates walks in
// nondecreasing length and the K-th time T is emitted gives the answer.
//
// The textbook A* here pushes one heap entry per out-edge of every popped node,
// which is up to N*K*outdeg entries and blows the 64MB limit on dense graphs (a
// 1000-node cycle padded with self-loops peaks near 1GB). Instead the candidates
// are kept factored: A[v] is the sorted list of the K smallest walk lengths S->v,
// and for a fixed in-edge (u,v,w) the stream A[u][0]+w, A[u][1]+w, ... is already
// sorted, so A[v] is a K-way merge over v's in-edges. Each node therefore owns a
// tiny min-heap over its in-edges (M entries in total), an edge blocked on a value
// of A[u] that does not exist yet parks in a waiting list that is flushed whenever
// A[u] grows, and one indexed heap over the N nodes (keyed by heap top + h[v])
// picks the globally smallest f. Every pop yields exactly one new value, so the
// whole run costs O((N*K + M) log) time and O(N*K + M) memory - about 8MB and
// well under a tenth of a second on the worst cases tried.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <vector>
#include <utility>

using namespace std;

const int MAXN = 1005;
const int MAXM = 100005;
const int INF = 0x3f3f3f3f;

int n, m, S, T, K;

int eu[MAXM], ev[MAXM], ew[MAXM];
int inStart[MAXN], outStart[MAXN];   // block bases, size n+2
int inBuf[MAXM];                     // per-node in-edge heap storage
int outBuf[MAXM];                    // per-node waiting-list storage
int hsz[MAXN], wsz[MAXN];
int nxt[MAXM];                       // next index into A[eu[e]] this edge consumes
int h[MAXN];                         // shortest distance node -> T
int Acnt[MAXN];
int *Av;                             // Av[v*K + i] = i-th smallest walk length s->v
char isDone[MAXN];

// ---- global heap over nodes, keyed by (top of H[v]) + h[v] ----
int gheap[MAXN], gpos[MAXN], gkey[MAXN], gsz;

inline int rd() {
    int c = getchar();
    while (c < '0' || c > '9') { if (c == EOF) return 0; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x;
}

inline int kf(int e) { return Av[eu[e] * K + nxt[e]] + ew[e]; }

// ---- per-node in-edge min-heap (on kf) ----
inline void hSiftUp(int v, int i) {
    int base = inStart[v];
    int e = inBuf[base + i], key = kf(e);
    while (i > 0) {
        int p = (i - 1) >> 1;
        int pe = inBuf[base + p];
        if (kf(pe) <= key) break;
        inBuf[base + i] = pe;
        i = p;
    }
    inBuf[base + i] = e;
}

inline void hSiftDown(int v, int i) {
    int base = inStart[v], sz = hsz[v];
    int e = inBuf[base + i], key = kf(e);
    for (;;) {
        int c = 2 * i + 1;
        if (c >= sz) break;
        if (c + 1 < sz && kf(inBuf[base + c + 1]) < kf(inBuf[base + c])) c++;
        int ce = inBuf[base + c];
        if (kf(ce) >= key) break;
        inBuf[base + i] = ce;
        i = c;
    }
    inBuf[base + i] = e;
}

inline void hPush(int v, int e) {
    inBuf[inStart[v] + hsz[v]] = e;
    hsz[v]++;
    hSiftUp(v, hsz[v] - 1);
}

inline void hPop(int v) {
    hsz[v]--;
    if (hsz[v] > 0) {
        inBuf[inStart[v]] = inBuf[inStart[v] + hsz[v]];
        hSiftDown(v, 0);
    }
}

// ---- global heap ----
inline void gSiftUp(int i) {
    int v = gheap[i], key = gkey[v];
    while (i > 0) {
        int p = (i - 1) >> 1;
        int pv = gheap[p];
        if (gkey[pv] <= key) break;
        gheap[i] = pv; gpos[pv] = i;
        i = p;
    }
    gheap[i] = v; gpos[v] = i;
}

inline void gSiftDown(int i) {
    int v = gheap[i], key = gkey[v];
    for (;;) {
        int c = 2 * i + 1;
        if (c >= gsz) break;
        if (c + 1 < gsz && gkey[gheap[c + 1]] < gkey[gheap[c]]) c++;
        int cv = gheap[c];
        if (gkey[cv] >= key) break;
        gheap[i] = cv; gpos[cv] = i;
        i = c;
    }
    gheap[i] = v; gpos[v] = i;
}

inline void gRemove(int v) {
    int i = gpos[v];
    if (i < 0) return;
    gpos[v] = -1;
    gsz--;
    if (i != gsz) {
        int last = gheap[gsz];
        gheap[i] = last; gpos[last] = i;
        gSiftDown(i);
        gSiftUp(gpos[last]);
    }
}

// Recompute v's key from its in-edge heap and place/move/remove it in the global heap.
inline void gUpdate(int v) {
    if (hsz[v] == 0 || isDone[v]) { gRemove(v); return; }
    int nk = kf(inBuf[inStart[v]]) + h[v];
    if (gpos[v] < 0) {
        gkey[v] = nk;
        gheap[gsz] = v; gpos[v] = gsz; gsz++;
        gSiftUp(gsz - 1);
    } else {
        int i = gpos[v];
        gkey[v] = nk;
        gSiftDown(i);
        gSiftUp(gpos[v]);
    }
}

// ---- Dijkstra on the reverse graph from T, using the raw in-edge lists ----
void dijkstra() {
    for (int i = 1; i <= n; i++) h[i] = INF;
    h[T] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push(make_pair(0, T));
    while (!pq.empty()) {
        pair<int, int> cur = pq.top(); pq.pop();
        int d = cur.first, v = cur.second;
        if (d > h[v]) continue;
        for (int i = inStart[v]; i < inStart[v + 1]; i++) {
            int e = inBuf[i], u = eu[e];
            if (d + ew[e] < h[u]) { h[u] = d + ew[e]; pq.push(make_pair(h[u], u)); }
        }
    }
}

int main() {
    n = rd(); m = rd();
    for (int e = 0; e < m; e++) { eu[e] = rd(); ev[e] = rd(); ew[e] = rd(); }
    S = rd(); T = rd(); K = rd();

    // The prince has to travel: when S == T the zero-length empty walk is not a path,
    // yet the search naturally produces it first, so ask for one more.
    if (S == T) K++;

    // block layout for in-edge and out-edge lists
    for (int i = 0; i <= n + 1; i++) { inStart[i] = 0; outStart[i] = 0; }
    for (int e = 0; e < m; e++) { inStart[ev[e] + 1]++; outStart[eu[e] + 1]++; }
    for (int i = 1; i <= n + 1; i++) { inStart[i] += inStart[i - 1]; outStart[i] += outStart[i - 1]; }
    {
        static int fill_[MAXN];
        for (int i = 0; i <= n + 1; i++) fill_[i] = 0;
        for (int e = 0; e < m; e++) { int v = ev[e]; inBuf[inStart[v] + fill_[v]++] = e; }
    }

    dijkstra();

    if (h[S] == INF) { printf("-1\n"); return 0; }

    Av = (int *)malloc(sizeof(int) * (size_t)(n + 1) * (size_t)K);
    if (!Av) { printf("-1\n"); return 0; }

    for (int v = 1; v <= n; v++) { hsz[v] = 0; wsz[v] = 0; Acnt[v] = 0; isDone[v] = 0; gpos[v] = -1; }
    gsz = 0;

    Acnt[S] = 1; Av[S * K] = 0;
    if (S == T && Acnt[T] == K) { printf("0\n"); return 0; }  // unreachable: K >= 2 here

    // seed: every usable edge either becomes active (source already has A[0]) or waits
    for (int e = 0; e < m; e++) {
        if (h[eu[e]] == INF || h[ev[e]] == INF) continue;
        nxt[e] = 0;
        if (eu[e] == S) hPush(ev[e], e);
        else outBuf[outStart[eu[e]] + wsz[eu[e]]++] = e;
    }
    for (int v = 1; v <= n; v++) if (hsz[v] > 0) gUpdate(v);

    while (gsz > 0) {
        int v = gheap[0];
        int e = inBuf[inStart[v]];
        int g = kf(e);
        hPop(v);

        // g is the next smallest walk length from S to v
        Av[v * K + Acnt[v]] = g;
        Acnt[v]++;
        if (v == T && Acnt[v] == K) { printf("%d\n", g); return 0; }
        if (Acnt[v] == K) { isDone[v] = 1; hsz[v] = 0; }

        // A[v] grew, so every out-edge of v that was blocked on it can advance
        {
            int base = outStart[v], cnt = wsz[v];
            wsz[v] = 0;
            for (int i = 0; i < cnt; i++) {
                int f = outBuf[base + i];
                int y = ev[f];
                if (isDone[y]) continue;      // y already has its K values
                hPush(y, f);
                if (y != v) gUpdate(y);
            }
        }

        // refill the edge we consumed
        int u = eu[e];
        nxt[e]++;
        if (!isDone[v]) {
            if (nxt[e] < Acnt[u]) hPush(v, e);
            else if (nxt[e] < K) outBuf[outStart[u] + wsz[u]++] = e;
        }

        gUpdate(v);
    }

    printf("-1\n");
    return 0;
}
