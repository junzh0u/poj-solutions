// POJ 2425 - A Chess Game
// Model: claude-opus-5
//
// Multi-pile impartial game on a DAG: each chess is an independent subgame
// (moving one chess along one out-edge is exactly one move of the sum), so by
// Sprague-Grundy the position is a first-player win iff the XOR of the Grundy
// values of the occupied positions is non-zero.
//
// Grundy value of a position is mex over its out-positions' Grundy values;
// sinks get 0, which encodes "the player who cannot move loses". Computed with
// an iterative DFS whose post-visit order guarantees every successor is final
// before a node's mex is taken - recursion here would need a per-frame mex
// mark array, which is the classic WA on this problem (a single global mark
// array gets clobbered by the recursive call). One shared mark array plus a
// stamp counter is used instead, so mex costs O(out-degree) with no clearing.
//
// Input framing the statement leaves implicit (confirmed on the discuss
// board): the file holds multiple test cases and ends at EOF, not with a
// sentinel; only the query list inside a case is terminated by a lone 0. A DAG
// on N <= 1000 nodes carries up to N(N-1)/2 edges, so the edge arrays grow
// dynamically rather than assuming a fixed cap - overflowing them shows up as
// Wrong Answer, not Runtime Error. Input is large, so it is parsed by hand.

#include <cstdio>
#include <cstdlib>

static const int MAXN = 5005;

static int head[MAXN];
static int *nxt;
static int *dst;
static int ecnt;
static int ecap;

static int sg[MAXN];
static int mark[MAXN + 2]; /* mex can reach n, so one slot past MAXN */
static int stampCnt;

static int stkNode[MAXN];
static int stkEdge[MAXN];
static int state[MAXN]; /* 0 = unvisited, 1 = on stack, 2 = done */

static void addEdge(int u, int v) {
    if (ecnt == ecap) {
        ecap = ecap ? ecap * 2 : 1024;
        nxt = (int *)realloc(nxt, (size_t)ecap * sizeof(int));
        dst = (int *)realloc(dst, (size_t)ecap * sizeof(int));
    }
    dst[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt;
    ++ecnt;
}

static char buf[1 << 16];
static int bufLen, bufPos;

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
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sign;
    return true;
}

int main() {
    int n;
    while (readInt(n)) {
        if (n < 0) n = 0;
        if (n > MAXN) n = MAXN;
        ecnt = 0;
        int i;
        for (i = 0; i < n; ++i) { head[i] = -1; state[i] = 0; sg[i] = 0; }
        for (i = 0; i < n; ++i) {
            int x;
            if (!readInt(x)) return 0;
            for (int j = 0; j < x; ++j) {
                int v;
                if (!readInt(v)) return 0;
                if (v >= 0 && v < n) addEdge(i, v);
            }
        }
        for (i = 0; i < n; ++i) {
            if (state[i] != 0) continue;
            int top = 0;
            stkNode[0] = i;
            stkEdge[0] = head[i];
            state[i] = 1;
            while (top >= 0) {
                int u = stkNode[top];
                int e = stkEdge[top];
                if (e != -1) {
                    stkEdge[top] = nxt[e];
                    int v = dst[e];
                    if (state[v] == 0) {
                        state[v] = 1;
                        ++top;
                        stkNode[top] = v;
                        stkEdge[top] = head[v];
                    }
                } else {
                    ++stampCnt;
                    for (int f = head[u]; f != -1; f = nxt[f]) mark[sg[dst[f]]] = stampCnt;
                    int m = 0;
                    while (mark[m] == stampCnt) ++m;
                    sg[u] = m;
                    state[u] = 2;
                    --top;
                }
            }
        }
        int m;
        while (readInt(m) && m != 0) {
            int res = 0;
            for (i = 0; i < m; ++i) {
                int p;
                if (!readInt(p)) break;
                if (p >= 0 && p < n) res ^= sg[p];
            }
            puts(res != 0 ? "WIN" : "LOSE");
        }
    }
    return 0;
}
